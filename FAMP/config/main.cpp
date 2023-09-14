#include "main.hpp"

int main(int args, char *argv[])
{

    if(strcmp((pint8) argv[1], "nm") == 0)
    {
        ConfigureFiles cf(FileToConfigure::ProtocolMakefile);
        cf.write_file();

        cf.set_new_file_being_configured(FileToConfigure::UserMakefile);
        cf.write_file();

        goto end;
    }

    if(strcmp((pint8) argv[1], "om") == 0)
    {
        ConfigureFiles cf(FileToConfigure::OldMakefile);
        cf.write_file();

        goto end;
    }

    if(strcmp((pint8) argv[1], "oldfs") == 0)
    {
        ConfigureFiles cf(FileToConfigure::OldFilesystem);
        cf.write_file();

        goto end;
    }

    if(strcmp((pint8) argv[1], "fkernel") == 0)
    {
        Formatter f(FileToFormat::Kernel);
        f.format();

        goto end;
    }

    if(strcmp((pint8) argv[1], "fmbr_part_table") == 0)
    {
        Formatter f(FileToFormat::MbrPartTable);
        f.format();

        goto end;
    }

    if(strcmp((pint8) argv[1], "fss") == 0)
    {
        Formatter f(FileToFormat::SecondStage);
        f.format();

        goto end;
    }
    
    if(strcmp((pint8) argv[1], "ffs_worker") == 0)
    {
        Formatter f(FileToFormat::FS_worker);
        f.format();

        goto end;
    }

    if(strcmp((pint8) argv[1], "format_fs") == 0)
    {
        ConfigureFiles cf(FileToConfigure::FileSystem);
        cf.write_file();

        goto end;
    }

    if(strcmp((pint8) argv[1], "disk_image") == 0)
    {
        FormatDiskImage fdimg(ProgramName::MBR);
        fdimg.write_program_to_disk_image();

        fdimg.switch_program(ProgramName::MBR_PART_TABLE_PROGRAM);
        fdimg.write_program_to_disk_image();

        fdimg.switch_program(ProgramName::FILESYSTEM);
        fdimg.write_program_to_disk_image();

        fdimg.switch_program(ProgramName::SECOND_STAGE);
        fdimg.write_program_to_disk_image();

        goto end;
    }

    if(strcmp((pint8) argv[1], "mbr") == 0)
    {
        ConfigureFiles cf(FileToConfigure::MBR);
        cf.write_file();

        goto end;
    }

    end:
    return 0;
}