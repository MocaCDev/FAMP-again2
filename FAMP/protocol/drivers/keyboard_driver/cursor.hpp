#ifndef FAMP_PROTOCOL_CURSOR_H
#define FAMP_PROTOCOL_CURSOR_H

/* Command ports for cursor. */
#define reg_screen_ctr  0x3D4
#define reg_screen_data 0x3D5

struct cursor
{
    uint32      pos_x;
    uint32      pos_y;

    cursor() = default;
    ~cursor() = default;
};

static struct cursor cursor_info = {.pos_x = 0, .pos_y = 0};

/* Update the cursor based on c_info.pos_x and c_info.pos_y. */
void __update_cursor_pos()
{
    uint16 pos = (c_info.pos_y * 80) + c_info.pos_x;
    __outp(reg_screen_ctr, 14);
    __outp(reg_screen_data, pos >> 8);
    __outp(reg_screen_ctr, 15);
    __outp(reg_screen_data, pos & 0xFF);
}

#endif