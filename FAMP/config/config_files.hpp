#ifndef FAMP_CONFIG_FILES_H
#define FAMP_CONFIG_FILES_H

using namespace YamlParser;

/*
 * The starting sector is 0x05 because the MBR part table program consists of only
 * three sectors.
 * */
uint16 starting_sector     = 0x02;

/*
 * This is the virtual address as to which the kernel will be located at.
 * We will add the number of bytes the kernel takes up to this to decipher the address
 * of the FS.
 * */
#define kernel_virtual_address 0x20000000

namespace ConfigFiles
{
    cpint8 mbr_format = (cpint8) initiate_path((pint8)"formats/", (pint8)"mbr_format");
    cpint8 protocol_makefile = (cpint8) initiate_path((pint8)"formats/", (pint8)"protocol_mf");
    cpint8 user_makefile = (cpint8) initiate_path((pint8)"formats/", (pint8)"user_mf");
    cpint8 old_mf = (cpint8) initiate_path((pint8)"formats/", (pint8)"old_mf");
    cpint8 fs_bin = (cpint8) initiate_path((pint8)"../bin/", (pint8)"fs.bin");
    cpint8 fs_worker_bin = (cpint8) initiate_path((pint8) "../bin/", (pint8)"fs_worker.bin");
    cpint8 second_stage_bin = (cpint8) initiate_path((pint8) "../bin/", (pint8)"second_stage.bin");
    cpint8 kernel_linker = (cpint8) initiate_path((pint8) "formats/", (pint8) "kernel_linker_format");
    cpint8 fs_linker = (cpint8) initiate_path((pint8)"formats/", (pint8)"fs_worker_linker_format");

    enum class FileToConfigure//: uint8_t
    {
        UserMakefile,
        ProtocolMakefile,
        MBR,
        OldMakefile,
        FileSystem,
        OldFilesystem,
        KernelLinker,
        FSLinker,
        None
    };

    class ConfigureFiles
    {
    private:
        FILE *config_file;
        FileToConfigure file_being_configured;

        /* Needed to get the yaml file data. */
        //yaml_parser &ypars = *new yaml_parser("../../boot.yaml");

        inline puint8 get_file_data()
        {
            FAMP_ASSERT(config_file,
                "\nAn error occurred with the format file that was opened prior. It is this, or the file was not found.\n")
            
            fseek(config_file, 0, SEEK_END);
            size_t format_size = ftell(config_file);
            fseek(config_file, 0, SEEK_SET);

            FAMP_ASSERT(format_size > 1,
                "\nThere was no data found in the format file.\n")
            
            puint8 format = new uint8[format_size];
            FAMP_ASSERT(format,
                "\nError allocating new memory for reading in the format.\n")
            
            FAMP_ASSERT(fread(format, sizeof(uint8), format_size, config_file) == format_size,
                "\nError reading in the needed %ld bytes.\n",
                format_size)
            
            return format;
        }
    
    public:
        ConfigureFiles(FileToConfigure ftc)
            : config_file(nullptr)
        {
            set_new_file_being_configured(ftc);
        }

        inline void set_new_file_being_configured(FileToConfigure ftc)
        {
            file_being_configured = ftc;

            if(config_file)
                fclose(config_file);
            
            switch(ftc)
            {
                case FileToConfigure::None: break;
                case FileToConfigure::KernelLinker: config_file = fopen(kernel_linker, "r");break;
                case FileToConfigure::FSLinker: config_file = fopen(fs_linker, "r");break;
                case FileToConfigure::UserMakefile: config_file = fopen(user_makefile, "r");break;
                case FileToConfigure::ProtocolMakefile: config_file = fopen(protocol_makefile, "r");break;
                case FileToConfigure::MBR: config_file = fopen(mbr_format, "r");break;
                case FileToConfigure::OldMakefile: config_file = fopen(old_mf, "r");break;
                case FileToConfigure::FileSystem:
                case FileToConfigure::OldFilesystem: config_file = fopen("formats/old_fs", "r");break;
                default: FAMP_ERROR("\nUnexpected argument passed to `ConfigureFiles` class constructor.\n")
            }
        }

        void write_file()
        {
            FILE *source_file = nullptr;
            bool needs_to_format = false;
            
            /* Based on `file_being_configured`, open the according source file. */
            switch(file_being_configured)
            {
                case FileToConfigure::KernelLinker: needs_to_format = true;source_file = fopen("../linker/kernel.ld", "w");break;
                case FileToConfigure::FSLinker: needs_to_format = true; source_file = fopen("../linker/fs_worker.ld", "w");break;
                case FileToConfigure::UserMakefile: source_file = fopen("../../Makefile", "w");break;
                case FileToConfigure::ProtocolMakefile: needs_to_format = true;source_file = fopen("../Makefile", "w");break;
                case FileToConfigure::MBR: needs_to_format = true;source_file = fopen("../boot/mbr.s", "w");break;
                case FileToConfigure::OldMakefile: source_file = fopen("../Makefile", "w");break;
                case FileToConfigure::FileSystem: needs_to_format = true;source_file = fopen("../boot/filesystem.s", "w");break;
                case FileToConfigure::OldFilesystem: source_file = fopen("../boot/filesystem.s", "w");break;
                default: FAMP_ERROR("\nAn unknown error occurred.\n")
            }

            puint8 format = get_file_data();
            
            if(needs_to_format)
            {
                uint8 completed_format[strlen((cpint8) format) + 120];
                memset(completed_format, 0, strlen((cpint8) format) + 120);

                if(file_being_configured == FileToConfigure::KernelLinker || file_being_configured == FileToConfigure::FSLinker)
                {
                    uint8 kernel_bin_path[40] = "../../%s";
                    uint8 abs_kernel_bin_path[40];

                    memset(abs_kernel_bin_path, 0, 40);

                    sprintf((pint8) abs_kernel_bin_path, (cpint8)kernel_bin_path,
                        yod.kernel_bin_filename);
                    
                    FILE *kernel_bin = fopen((cpint8)abs_kernel_bin_path, "rb");

                    fseek(kernel_bin, 0, SEEK_END);
                    size_t kernel_bin_size = ftell(kernel_bin);
                    fseek(kernel_bin, 0, SEEK_SET);
                    fclose(kernel_bin);

                    sprintf((pint8) completed_format, (cpint8)format, kernel_virtual_address + kernel_bin_size);
                    goto write;
                }

                if(file_being_configured == FileToConfigure::ProtocolMakefile)
                {
                    sprintf((pint8) completed_format, (cpint8) format,
                        yod.kernel_bin_filename, yod.kernel_bin_filename, 
                        yod.kernel_bin_filename, yod.kernel_o_filename,
                        yod.kernel_bin_filename,
                        yod.kernel_o_filename, yod.kernel_source_filename,
                        yod.kernel_o_filename, yod.kernel_bin_filename,
                        yod.bin_folder, yod.bin_folder
                    );
                    
                    goto write;
                }

                if(file_being_configured == FileToConfigure::FileSystem)
                {
                    sprintf((pint8) completed_format, (cpint8) format,
                        yod.kernel_bin_filename);
                    
                    goto write;
                }

                if(file_being_configured == FileToConfigure::MBR)
                {
                    FILE *fbin = nullptr;

                    const auto get_file_size = [&fbin] ()
                    {
                        FAMP_ASSERT(fbin,
                            "\nError with the file.\n")
                        
                        fseek(fbin, 0, SEEK_END);
                        size_t file_size = ftell(fbin);
                        fseek(fbin, 0, SEEK_SET);

                        return (size_t) file_size;
                    };

                    const auto open_file = [&fbin] (cpint8 filename, cpint8 mode)
                    {
                        fbin = fopen(filename, mode);
                        FAMP_ASSERT(fbin,
                            "\nError opening the binary file `%s`.\n",
                            filename)
                    };

                    /* Formatting FS binary to be multiple of 512 bytes. */
                    /*{
                        open_file(fs_bin, (cpint8) "rb");
                        
                        size_t fs_bin_size = get_file_size();

                        size_t bytes_needed = fs_bin_size;
                        while(bytes_needed % 512 != 0)
                            bytes_needed++;
                        
                        uint8 padding[bytes_needed - fs_bin_size];
                        memset(padding, 0, bytes_needed - fs_bin_size);
                        puint8 fs_data = new uint8[fs_bin_size];
                        fread(fs_data, fs_bin_size, sizeof(*fs_data), fbin);

                        fclose(fbin);

                        {
                            open_file(fs_bin, (cpint8) "wb");
                            fwrite(fs_data, fs_bin_size, sizeof(*fs_data), fbin);
                            fwrite(&padding, bytes_needed - fs_bin_size, sizeof(uint8), fbin);
                            fclose(fbin);
                        }

                        delete fs_data;
                    }*/

                    /* Get the filesystem binary size. */
                    open_file(fs_bin, "rb");
                    size_t fbin_size = get_file_size();
                    fclose(fbin);

                    open_file(second_stage_bin, "rb");
                    size_t ssbin_size = get_file_size();
                    fclose(fbin);
                    starting_sector += (ssbin_size / 512);

                    open_file(fs_worker_bin, "rb");
                    size_t fs_worker_bin_size = get_file_size();
                    fclose(fbin);

                    uint8 os_name[14];
                    memset(os_name, ' ', 14);
                    memcpy(os_name, yod.OS_name, strlen((cpint8) yod.OS_name));
                    os_name[14] = '\0';

                    /* TODO: This needs some work. */
                    sprintf((pint8) completed_format, (cpint8) format,
                        os_name, yod.type, yod.OS_version,
                        yod.OS_vid_mode, yod.FS_type, yod.in_production,
                        0x02 + (ssbin_size / 512),
                        (ssbin_size / 512),
                        starting_sector,
                        starting_sector + (fbin_size / 512),
                        (fbin_size / 512),
                        (starting_sector + (fbin_size / 512)),
                        ((starting_sector + (fbin_size / 512)) + (fs_worker_bin_size / 512)),
                        (fs_worker_bin_size / 512));

                    goto write;
                }
                
                write:
                fwrite(completed_format, sizeof(uint8), strlen((cpint8) completed_format), source_file);
                goto end;
            }

            fwrite(format, sizeof(uint8), strlen((cpint8) format), source_file);
            
            end:
            fclose(source_file);
            delete format;
        }

        ~ConfigureFiles()
        {
            if(config_file) fclose(config_file);
            config_file = nullptr;
        }
    };
}

#endif