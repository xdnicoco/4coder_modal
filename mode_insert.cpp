/*
This mode is used to insert text into the buffer.

WARNING:
This custom extension provided "as is" without warranty of any kind,
 either express or implied, including without limitation any implied warranties of condition,
 uninterrupted use, merchantability, fitness for a particular purpose, or non-infringement.
*/

#if !defined(_MODE_INSERT_CPP)
#define _MODE_INSERT_CPP

struct NJ_MODE_STATE_DECLERATION(NJ_CURRENT_MODE) {};

#define NJ_MODE_PRINT_ENTER_HOOK
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x100f05, // color_bg
                             0x711400, // color_bar
                             0x752410, // color_bar_hover
                             0x793420, // color_bar_active
                             0x793420, // color_mode
                             0x8d360d, // color_mark
                             0x55cccc, // color_pop1
                             0x50080f  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_mode_enter_insert)
CUSTOM_DOC("Activates 'insert' mode.")
{
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    bind(context, '\t', MDFR_NONE,  word_complete);
    bind(context, '\t', MDFR_CTRL,  auto_tab_range);
    bind(context, '\t', MDFR_SHIFT, auto_tab_line_at_cursor);
    bind(context, key_back, MDFR_SHIFT, backspace_char);
    bind_vanilla_keys(context, write_character);
    inherit_map(context, mapid_common);
    end_map(context); // mapid_insert
}


CUSTOM_COMMAND_SIG(nj_newline_then_insert_before)
CUSTOM_DOC("Inserts a newline before the line under the cursor, then activates insert mode."){
    seek_beginning_of_line(app);
    write_string(app, make_lit_string("\n"));
    move_left(app);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE)
}

CUSTOM_COMMAND_SIG(nj_newline_then_insert_after)
CUSTOM_DOC("Inserts a newline after the line under the cursor, then activates insert mode."){
    seek_end_of_line(app);
    write_string(app, make_lit_string("\n"));
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE)
}

CUSTOM_COMMAND_SIG(nj_insert_after)
CUSTOM_DOC("Moves to the next character in the current line, then activates insert mode."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    char nextch[2];
    int pos = view.cursor.pos;
    buffer_read_range(app, &buffer, pos, pos + 1, nextch);
    if (nextch[0] != '\n') {
        move_right(app);
    }
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE)
}

CUSTOM_COMMAND_SIG(nj_seek_eol_then_insert)
CUSTOM_DOC("Moves to the end of the current line, then activates insert mode."){
    seek_end_of_textual_line(app);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE)
}

#endif // _MODE_INSERT_CPP