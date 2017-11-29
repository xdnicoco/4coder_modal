#if !defined(NJ_MODE_CHORD_GOTO_CPP)
#define NJ_MODE_CHORD_GOTO_CPP


enum NJ_Chord_Goto_State_Mode {
    nj_chord_goto_mode_absolute = 0,
    nj_chord_goto_mode_add,
    nj_chord_goto_mode_sub,
    nj_chord_goto_mode_cancel,
};

struct NJ_MODE_STATE_DECLERATION(NJ_CURRENT_MODE){
    int32_t line_input;
    Full_Cursor last_position;
    NJ_Chord_Goto_State_Mode goto_mode;
};

#define NJ_MODE_PRINT_ENTER_HOOK \
View_Summary view = get_active_view(app, AccessProtected);\
NJ_MODE_STATE(NJ_CURRENT_MODE).line_input = 0;\
NJ_MODE_STATE(NJ_CURRENT_MODE).last_position = view.cursor;\
NJ_MODE_STATE(NJ_CURRENT_MODE).goto_mode = nj_chord_goto_mode_absolute;

NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x051510, // color_bg
                             0x034449, // color_bar
                             0x021f4f, // color_bar_hover
                             0x055459, // color_bar_active
                             0x207469, // color_mode
                             0x0d6695, // color_mark
                             0x03cf0c, // color_pop1
                             0xff0000  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_mode_enter_chord_goto)
CUSTOM_DOC("Activates 'chord goto' mode.")
{
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    inherit_map(context, mapid_nomap);
    bind(context,'0', MDFR_NONE, nj_chord_goto_seek_line);
    bind(context,'1', MDFR_NONE, nj_chord_goto_seek_line);
    bind(context,'2', MDFR_NONE, nj_chord_goto_seek_line);
    bind(context,'3', MDFR_NONE, nj_chord_goto_seek_line);
    bind(context,'4', MDFR_NONE, nj_chord_goto_seek_line);
    bind(context,'5', MDFR_NONE, nj_chord_goto_seek_line);
    bind(context,'6', MDFR_NONE, nj_chord_goto_seek_line);
    bind(context,'7', MDFR_NONE, nj_chord_goto_seek_line);
    bind(context,'8', MDFR_NONE, nj_chord_goto_seek_line);
    bind(context,'9', MDFR_NONE, nj_chord_goto_seek_line);
    
    bind(context,'+', MDFR_NONE, nj_chord_goto_set_mode_add);
    bind(context,'-', MDFR_NONE, nj_chord_goto_set_mode_sub);
    bind(context,'=', MDFR_NONE, nj_chord_goto_set_mode_absolute);
    
    bind(context,'g', MDFR_NONE, nj_chord_goto_seek_beginning_of_file_then_prev);
    bind(context,'G', MDFR_NONE, nj_chord_goto_seek_end_of_file_then_prev);
    bind(context, key_back, MDFR_NONE, nj_chord_goto_backspace);
    
    bind(context, key_esc, MDFR_NONE, nj_chord_goto_cancel);
    
    bind(context, '\n', MDFR_NONE, nj_activate_previous_mode);
    end_map(context);
}

inline void nj_chord_goto_apply_seek(Application_Links *app){
    uint32_t line_number;
    View_Summary view = get_active_view(app, AccessProtected);
    
    switch(NJ_MODE_STATE(NJ_CURRENT_MODE).goto_mode){
        case nj_chord_goto_mode_absolute:{
            line_number = NJ_MODE_STATE(NJ_CURRENT_MODE).line_input;
        } break;
        case nj_chord_goto_mode_add:{
            line_number = NJ_MODE_STATE(NJ_CURRENT_MODE).last_position.line + NJ_MODE_STATE(NJ_CURRENT_MODE).line_input;
        } break;
        case nj_chord_goto_mode_sub:{
            line_number = NJ_MODE_STATE(NJ_CURRENT_MODE).last_position.line - NJ_MODE_STATE(NJ_CURRENT_MODE).line_input;
        } break;
        default:
        case nj_chord_goto_mode_cancel:{
            view_set_cursor(app, &view, seek_pos(NJ_MODE_STATE(NJ_CURRENT_MODE).last_position.pos), true);
            return;
        } break;
    }
    
#if 0
    char msg[1024];
    sprintf(msg, "Goto state: {line_input = %5u, line_prev = %5u, mode = %d}, line_number = %u\n", nj_chord_goto_state.line_input, nj_chord_goto_state.line_prev, nj_chord_goto_state.mode, line_number);
    print_message(app, msg, str_size(msg));
#endif
    
    view_set_cursor(app, &view, seek_line_char(line_number, NJ_MODE_STATE(NJ_CURRENT_MODE).last_position.character), true);
}

CUSTOM_COMMAND_SIG(nj_chord_goto_seek_end_of_file_then_prev)
CUSTOM_DOC("Places the cursor at the end of the buffer, then activate the previous mode."){
    exec_command(app, nj_seek_end_of_file);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_chord_goto_seek_beginning_of_file_then_prev)
CUSTOM_DOC("Places the cursor at the beggining of the buffer, then activate the previous mode."){
    exec_command(app, nj_seek_beginning_of_file);
    exec_command(app, nj_activate_previous_mode);
}

CUSTOM_COMMAND_SIG(nj_chord_goto_seek_line)
CUSTOM_DOC("If the character used to trigger this command is numeric, adds it to the goto state and calculates the target line number."){
    User_Input in = get_command_input(app);
    
    if(char_is_numeric((char)in.key.character))
    {
        char input_character[] = {(char)in.key.character, 0}; // IMPORTANT(NJ): NULL terminate your c strings :(
        i32_4tech input_digit = str_to_int_c(input_character);
        
#if 0
        char msg[256];
        sprintf(msg, "\ninput_digit  = %d\n",input_digit);
        print_message(app, msg, str_size(msg));
#endif
        NJ_MODE_STATE(NJ_CURRENT_MODE).line_input = NJ_MODE_STATE(NJ_CURRENT_MODE).line_input*10 + input_digit;
        nj_chord_goto_apply_seek(app);
    }
}

CUSTOM_COMMAND_SIG(nj_chord_goto_set_mode_add)
CUSTOM_DOC("Sets the goto chord mode to addition, then calculates the target line number."){
    NJ_MODE_STATE(NJ_CURRENT_MODE).goto_mode = nj_chord_goto_mode_add;
    nj_chord_goto_apply_seek(app);
}

CUSTOM_COMMAND_SIG(nj_chord_goto_set_mode_sub)
CUSTOM_DOC("Sets the goto chord mode to subtraction, then calculates the target line number."){
    NJ_MODE_STATE(NJ_CURRENT_MODE).goto_mode = nj_chord_goto_mode_sub;
    nj_chord_goto_apply_seek(app);
}

CUSTOM_COMMAND_SIG(nj_chord_goto_set_mode_absolute)
CUSTOM_DOC("Sets the goto chord mode to absolute, then calculates the target line number."){
    NJ_MODE_STATE(NJ_CURRENT_MODE).goto_mode = nj_chord_goto_mode_absolute;
    nj_chord_goto_apply_seek(app);
}

CUSTOM_COMMAND_SIG(nj_chord_goto_backspace)
CUSTOM_DOC("Devides the goto line number input by 10, then calculates the target line number."){
    NJ_MODE_STATE(NJ_CURRENT_MODE).line_input /= 10;
    nj_chord_goto_apply_seek(app);
}

CUSTOM_COMMAND_SIG(nj_chord_goto_cancel)
CUSTOM_DOC("Seek back to the line number before entering chord goto mode, then activate the previous mode."){
    NJ_MODE_STATE(NJ_CURRENT_MODE).goto_mode = nj_chord_goto_mode_cancel;
    nj_chord_goto_apply_seek(app);
    exec_command(app, nj_activate_previous_mode);
}

#endif // NJ_MODE_CHORD_GOTO_CPP
