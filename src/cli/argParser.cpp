#include "argParser.h"
#include <cxxopts.hpp>
#include "../steganography/messenger.h"
#include "../images/imageLoader.h"

void parseArguments(int argc, char** argv)
{
    cxxopts::Options options("Steg", "Steganography Tool");

    options.add_options()
        ("h,help", "Show help")
        ("command", "The command to execute: read | readall | write ", cxxopts::value<std::string>())
        ("input", "Path of the target image", cxxopts::value<std::string>())
        ("o,output", "Path of the output image. Only valid when using write", cxxopts::value<std::string>())
        ("m,message", "Content of the message. Only valid when using write", cxxopts::value<std::string>())
        ("i,index", "Index of the message you want to read. Only valid when using read", cxxopts::value<size_t>());

    options.positional_help("<command> <input file>");
    options.parse_positional({ "command", "input" });

    cxxopts::ParseResult result;

    try
    {
        result = options.parse(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << options.help() << std::endl;
        return;
    }

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return;
    }

    std::string command;
    if (!result.count("command"))
    {
        std::cerr << "Please specify command: read | readall | write" << std::endl;
        std::cout << options.help() << std::endl;
        return;
    }
    command = result["command"].as<std::string>();

    std::string input_path;
    if (!result.count("input"))
    {
        std::cerr << "Please specify input image" << std::endl;
        std::cout << options.help() << std::endl;
        return;
    }
    input_path = result["input"].as<std::string>();

    std::string output_path;
    if (!result.count("output"))
        output_path = "m_" + input_path;
    else
        output_path = result["output"].as<std::string>();

    std::string message = "";
    if (result.count("message"))
        message = result["message"].as<std::string>();

    size_t message_index = 0;
    if (result.count("index"))
        message_index = result["index"].as<size_t>();

    ImageLoader imageLoader;
    std::unique_ptr<Image> image;
    try
    {
        image = imageLoader.openImage(input_path);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }

    Messenger messenger;

    if (command == "read")
    {
        try
        {
            std::cout << messenger.readMessage(*image, message_index) << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return;
        }
    }
    else if (command == "readall")
    {
        std::vector<std::string> messages;
        try
        {
            messages = messenger.readAllMessages(*image);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return;
        }
        
        for (std::string message : messages)
        {
            std::cout << message << std::endl;
        }
    }
    else if (command == "write")
    {
        if (message == "")
        {
            std::cerr << "When writing, please specify message content" << std::endl;
            std::cout << options.help() << std::endl;
            return;
        }

        try
        {
            messenger.writeMessage(*image, message);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return;
        }

        imageLoader.saveImage(*image, output_path);
    }
    else
    {
        std::cout << options.help() << std::endl;
    }
}