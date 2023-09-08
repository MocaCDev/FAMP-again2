#ifndef FAMP_PROTOCOL_CONFIG_DISK_IMAGE_H
#define FAMP_PROTOCOL_CONFIG_DISK_IMAGE_H

namespace ConfigDiskImage
{
    enum class ProgramName
    {
        MBR,
        MBR_PART_TABLE_PROGRAM,
        SECOND_STAGE,
        FS_WORKER,
        FILESYSTEM
    };

    /* File paths. */
    cpint8 mbr_path = (cpint8) initiate_path((pint8)"../bin/", (pint8) "mbr.bin");
    cpint8 mbr_part_table_path = (cpint8) initiate_path((pint8) "../bin/", (pint8) "mbr_part_table.bin");
    cpint8 second_stage_path = (cpint8) initiate_path((pint8) "../bin/", (pint8) "second_stage.bin");
    cpint8 fs_worker_path = (cpint8) initiate_path((pint8) "../bin/", (pint8) "fs_worker.bin");
    cpint8 fs_path = (cpint8) initiate_path((pint8) "../bin/", (pint8) "fs.bin");

    class FormatDiskImage
    {
    private:
        ProgramName pname;
        FILE *disk_image = fopen("../bin/OS.img", "wb");
        FILE *source;
        puint8 program_data;
    
    public:
        FormatDiskImage(ProgramName program)
            : pname(program), source(nullptr), program_data(nullptr)
        {
        }

        void switch_program(ProgramName program)
        {
            FAMP_ASSERT(program != pname,
                "\nCannot switch the program to the same program that was currently being worked with.\n")
            
            pname = program;

            if(program_data) delete program_data;
        }

        void write_program_to_disk_image()
        {
            switch(pname)
            {
                case ProgramName::MBR: source = fopen(mbr_path, "rb");break;
                case ProgramName::MBR_PART_TABLE_PROGRAM: source = fopen(mbr_part_table_path, "rb");break;
                case ProgramName::SECOND_STAGE: source = fopen(second_stage_path, "rb"); break;
                case ProgramName::FS_WORKER: source = fopen(fs_worker_path, "rb"); break;
                case ProgramName::FILESYSTEM: source = fopen(fs_path, "rb"); break;
                default: FAMP_ERROR("\nUnknown program name.\n")
            }

            fseek(source, 0, SEEK_END);
            size_t binary_size = ftell(source);
            fseek(source, 0, SEEK_SET);

            FAMP_ASSERT(binary_size > 1,
                "\nThe binary file was found with not enough data.\n")
            
            program_data = new uint8[binary_size];
            fread(program_data, binary_size, sizeof(*program_data), source);
            fclose(source);

            fwrite(program_data, binary_size, sizeof(*program_data), disk_image);
        }

        ~FormatDiskImage()
        {
            if(disk_image) fclose(disk_image);
            if(source) fclose(source);
            if(program_data) delete program_data;

            free((puint8) mbr_path);
            free((puint8) mbr_part_table_path);
            free((puint8) second_stage_path);
            free((puint8) fs_worker_path);
            free((puint8) fs_path);
        }
    };
}

#endif