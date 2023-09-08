#ifndef FAMP_PROTOCOL_FILE_FORMATTER_H
#define FAMP_PROTOCOL_FILE_FORMATTER_H
#include <FFF/FFF_structures.hpp>

namespace FileFormatter
{
    enum class FileToFormat
    {
        Kernel,
        SecondStage,
        MbrPartTable
    };

    class Formatter
    {
    private:
        FileToFormat file_tf;
        FILE *binary;

    public:
        Formatter(FileToFormat ftf)
            : file_tf(ftf), binary(nullptr)
        {}

        void format()
        {
            puint8 file_data = nullptr;

            const auto open_file = [this] (cpint8 filename, cpint8 mode)
            {
                binary = fopen(filename, mode);

                FAMP_ASSERT(binary,
                    "\nError opening the files `%s`.\n",
                    filename)
            };

            const auto get_file_size = [this] ()
            {
                fseek(binary, 0, SEEK_END);
                size_t file_size = ftell(binary);
                fseek(binary, 0, SEEK_SET);

                FAMP_ASSERT(file_size > 1,
                    "\nThe binary file has no data.\n")
                
                return (size_t) file_size;
            };

            const auto get_file_data = [&file_data, this] (size_t file_size)
            {
                if(file_data) delete file_data;
                file_data = new uint8[file_size];

                fread(file_data, file_size, sizeof(*file_data), binary);
            };

            const auto pad_binary = [&get_file_size, &get_file_data, &open_file, &file_data, this] (cpint8 filename, size_t preferred_size)
            {
                size_t file_size = get_file_size();
                
                /* Get amount of bytes that are needed to make the binary file a multiple of 512 bytes. */
                size_t bytes_needed = file_size;
                
                if(preferred_size != 0)
                    bytes_needed = preferred_size - bytes_needed;

                while(bytes_needed % 512 != 0)
                    bytes_needed++;
                
                uint8 padding[bytes_needed - file_size];
                memset(padding, 0, bytes_needed - file_size);

                get_file_data(file_size);

                fclose(binary);

                {
                    open_file(filename, "wb");
                    fwrite(file_data, file_size, sizeof(*file_data), binary);
                    fwrite(&padding, bytes_needed - file_size, sizeof(uint8), binary);
                    fclose(binary);
                }
            };

            switch(file_tf)
            {
                case FileToFormat::Kernel: {
                    uint8 kernel_bin_path[40] = "../../%s";
                    uint8 abs_kernel_bin_path[40];

                    memset(abs_kernel_bin_path, 0, 40);

                    sprintf((pint8) abs_kernel_bin_path, (cpint8)kernel_bin_path,
                        yod.kernel_bin_filename);

                    open_file((cpint8) abs_kernel_bin_path, "rb");
                    pad_binary((cpint8) abs_kernel_bin_path, 0);

                    delete file_data;
                    return;
                }
                case FileToFormat::SecondStage: {
                    open_file((cpint8) "../bin/second_stage.bin", "rb");
                    pad_binary((cpint8) "../bin/second_stage.bin", 0);
                    delete file_data;

                    return;
                }
                case FileToFormat::MbrPartTable: {
                    open_file((cpint8) "../bin/mbr_part_table.bin", "rb");
                    pad_binary((cpint8) "../bin/mbr_part_table.bin", 1536);
                    delete file_data;

                    return;
                }
                default: FAMP_ERROR("Unexpected value for FileToFormat.\n")
            }
        }

        ~Formatter() = default;
    };
}

#endif