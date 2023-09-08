#ifndef FAMP_PROTOCOL_MAIN_H
#define FAMP_PROTOCOL_MAIN_H
#include <common.hpp>
#include "../YamlParser/parser.hpp"
#include "formatter.hpp"
#include "config_files.hpp"
#include "config_disk_image.hpp"

using namespace YamlParser;
using namespace FileFormatter;
using namespace ConfigFiles;
using namespace ConfigDiskImage;

yaml_parser &ypars = *new yaml_parser("../../boot.yaml");

int main(int args, char *argv[]);

#endif