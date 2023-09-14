#ifndef FAMP_PROTOCOL_FILE_FORMATTER_H
#define FAMP_PROTOCOL_FILE_FORMATTER_H
#include <FFF/FFF_structures.hpp>

using namespace FFF_Structures;

namespace FileFormatter
{
    enum class FileToFormat
    {
        Kernel,
        SecondStage,
        MbrPartTable,
        FS_worker
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
            struct FAMP_PROTOCOL_SUBHEADING subheading;
            struct FAMP_PROTOCOL_MEMORY_STAMP mem_stamp;


            /*
             * TODO: With the following variable, we need a way to know if a program
             *       is an assembly program or not. There are some programs that we
             *       know the definite answer of, however the users will have programs
             *       they store on the FS, and they could be assembly programs.
             *       So, with this, there will need to be some sort of indication (assembly program header)
             *       that the program is, in fact, a assembly program.
             *
             *       Later on, the protocol will support assembly in the real mode/long mode OS.
             *       It will offer an assembly file that stores the header info that an assembly
             *       program will need so it is noticed as an assembly program later on.
             *
             *       Odds are, as the protocol advances this will eventually change and adapt
             *       to a much better structure, but for now this is how the protocol (and the FS)
             *       will decipher an assembly program from a program written in a higher-level langauge.
             *
             * */
            bool is_asm_program = false;

            const auto init_mem_stamp = [&mem_stamp] (uint16 mem_id)
            {
                mem_stamp.MemID = mem_id;

                return;
            };

            const auto init_subheading = [&subheading, &is_asm_program] (uint32 psize_in_bytes, uint16 psize_in_sectors)
            {
                subheading.SubHeadingSig = FAMP_SUBHEADER_SIGNATURE;
                subheading.ProgramSizeInBytes = psize_in_bytes;
                subheading.ProgramSizeInSectors = psize_in_sectors;
                subheading.IsAsmProgram = is_asm_program;

                return;
            };

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

            const auto pad_binary = [&get_file_size, &get_file_data, &open_file, &file_data, &subheading, &mem_stamp, this] (cpint8 filename, size_t preferred_size)
            {
                size_t file_size = get_file_size();
                
                /* Get amount of bytes that are needed to make the binary file a multiple of 512 bytes. */
                size_t bytes_needed = file_size;
                
                if(preferred_size != 0)
                    bytes_needed = preferred_size - bytes_needed;

                while(bytes_needed % 512 != 0)
                    bytes_needed++;
                
                if(mem_stamp.MemID != 0x0)
                    bytes_needed -= sizeof(mem_stamp);
                
                if(subheading.SubHeadingSig != 0x0)
                    bytes_needed -= sizeof(subheading);

                
                printf("%d, %d\n", bytes_needed, file_size);
                
                uint8 padding[bytes_needed - file_size];
                memset(padding, 0, bytes_needed - file_size);

                get_file_data(file_size);

                fclose(binary);

                {
                    open_file(filename, "wb");
                    if(subheading.SubHeadingSig != 0x0) fwrite(&subheading, 1, sizeof(subheading), binary);
                    fwrite(file_data, file_size, sizeof(*file_data), binary);
                    fwrite(&padding, bytes_needed - file_size, sizeof(uint8), binary);
                    if(mem_stamp.MemID != 0x0) fwrite((const void *)&mem_stamp, 1, sizeof(mem_stamp), binary);
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

                    init_mem_stamp(FAMP_MEM_STAMP_KERNEL);
                    pad_binary((cpint8) abs_kernel_bin_path, 0);

                    delete file_data;

                    mem_stamp.MemID = 0x0;

                    return;
                }
                case FileToFormat::SecondStage: {
                    
                    open_file((cpint8) "../bin/second_stage.bin", "rb");
                    
                    init_subheading(get_file_size(), get_file_size()/512);
                    init_mem_stamp(FAMP_MEM_STAMP_SECOND_STAGE);
                    pad_binary((cpint8) "../bin/second_stage.bin", 0);
                    
                    delete file_data;
                    //delete mem_stamp;
                    
                    subheading.SubHeadingSig = 0x0;
                    mem_stamp.MemID = 0x0;

                    return;
                }
                case FileToFormat::MbrPartTable: {
                    
                    open_file((cpint8) "../bin/mbr_part_table.bin", "rb");
                    
                    init_mem_stamp(FAMP_MEM_STAMP_MBR_PTBL);
                    pad_binary((cpint8) "../bin/mbr_part_table.bin", 1536);
                    
                    delete file_data;

                    mem_stamp.MemID = 0x0;

                    return;
                }
                case FileToFormat::FS_worker: {
                    open_file((cpint8) "../bin/fs_worker.bin", "rb");
                    pad_binary((cpint8) "../bin/fs_worker.bin", 1024);

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