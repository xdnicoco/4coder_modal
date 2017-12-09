/*
This mode replaces the character under the cursor with
 the next character pressed after activating it, then returns to the previous mode.
 
WARNING:
This custom extension provided "as is" without warranty of any kind,
 either express or implied, including without limitation any implied warranties of condition,
 uninterrupted use, merchantability, fitness for a particular purpose, or non-infringement.
*/

#if !defined(_MODE_CHORD_REPLACE_SINGLE_CPP)
#define _MODE_CHORD_REPLACE_SINGLE_CPP

#define NJ_MODE_PRINT_ENTER_HOOK
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x050f15, // color_bg
                             0x032459, // color_bar
                             0x1c338e, // color_bar_active
                             0x1e439e, // color_bar_hover
                             0xe91e63, // color_mode
                             0xc30c4a, // color_mark
                             0x03cf0c, // color_pop1
                             0x50080f  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_mode_enter_chord_replace_single)
CUSTOM_DOC("Activates 'chord replace single' mode.")
{
    NJ_ENTER_MODE(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    inherit_map(context, mapid_nomap);
    bind_vanilla_keys(context, nj_replace_character_then_prev);
    bind(context, 'v', MDFR_CTRL, nj_mode_chord_replace_single_paste_then_prev);
    bind(context, key_esc, MDFR_NONE, nj_mode_enter_normal);
    end_map(context);
}

CUSTOM_COMMAND_SIG(nj_replace_character_then_prev)
CUSTOM_DOC("Replaces the character under the cursor with the pressed character, then return to the previous mode."){
    nj_replace_character(app);
    move_left(app);
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_mode_chord_replace_single_paste_then_prev)
CUSTOM_DOC("Removes the character under the cursor, then paste from the clipboard and return to the previous mode."){
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    if(buffer_get_char(app, &buffer, view.cursor.pos) != '\n') {
        delete_char(app);
    }
    paste(app);
    nj_activate_previous_mode(app);
}


#endif // _MODE_CHORD_REPLACE_SINGLE_CPP
