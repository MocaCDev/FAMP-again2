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
        /* TODO: Make it to where all the programs that run are before any program that
        *        just stores data (ProgramName::FILESYSTEM).
        *
        * The order should be:
        *   ProgramName::MBR
        *   ProgramName::MBR_PART_TABLE_PROGRAM
        *   ProgramName::SECOND_STAGE
        *   ProgramName::FILESYSTEM
        *
        * Too lazy to fix it right now, will do tomorrow.
        * This change will require some tweaks to the MBR partition table entries
        * alongside some changes to the file `config_files.hpp`.
        *
        * What I might do is make the second stage bootloader an assembly program
        * for a little more control over things. I also believe writing the second stage bootloader
        * in assembly will make things go smoother with how the overall OS is structured (memory wise).
        *
        * */
        FormatDiskImage fdimg(ProgramName::MBR);
        fdimg.write_program_to_disk_image();

        //fdimg.switch_program(ProgramName::MBR_PART_TABLE_PROGRAM);
        //fdimg.write_program_to_disk_image();

        fdimg.switch_program(ProgramName::SECOND_STAGE);
        fdimg.write_program_to_disk_image();

        fdimg.switch_program(ProgramName::FILESYSTEM);
        fdimg.write_program_to_disk_image();

        fdimg.switch_program(ProgramName::FS_WORKER);
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