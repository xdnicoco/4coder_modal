/*
This mode binds the functions to set up the case (upper, lower, or camel)
 of the token or the selected range.
 
WARNING:
This custom extension provided "as is" without warranty of any kind,
 either express or implied, including without limitation any implied warranties of condition,
 uninterrupted use, merchantability, fitness for a particular purpose, or non-infringement.
*/

#if !defined(_MODE_CHORD_CASE_CPP)
#define _MODE_CHORD_CASE_CPP

#define NJ_MODE_PRINT_ENTER_HOOK
NJ_MODE_PRINT_ENTER_FUNCTION(NJ_CURRENT_MODE,
                             0x100f05, // color_bg
                             0x412410, // color_bar
                             0x453420, // color_bar_hover
                             0x494430, // color_bar_active
                             0x593450, // color_mode
                             0x8d360d, // color_mark
                             0x55cccc, // color_pop1
                             0x50080f  // color_pop2
                             );
#undef NJ_MODE_PRINT_ENTER_HOOK

CUSTOM_COMMAND_SIG(nj_mode_enter_chord_case)
CUSTOM_DOC("Activates 'chord case' mode.")
{
    NJ_ENTER_MODE(NJ_CURRENT_MODE);
}

NJ_MODE_BIND_DECLERATION(NJ_CURRENT_MODE){
    begin_map(context, NJ_MODE_MAPID(NJ_CURRENT_MODE));
    inherit_map(context, mapid_nomap);
    bind(context, 'l', MDFR_NONE, nj_chord_case_lower_token_or_word);
    bind(context, 'L', MDFR_NONE, nj_chord_case_lower);
    bind(context, 'u', MDFR_NONE, nj_chord_case_upper_token_or_word);
    bind(context, 'U', MDFR_NONE, nj_chord_case_upper);
    bind(context, 'c', MDFR_NONE, nj_chord_case_camel_token_or_word);
    bind(context, 'C', MDFR_NONE, nj_chord_case_camel);
    bind(context, key_esc, MDFR_NONE, nj_mode_enter_normal);
    end_map(context); // mapid_insert
}


CUSTOM_COMMAND_SIG(nj_chord_case_upper)
CUSTOM_DOC("Converts all ascii text in the range between the cursor and the mark to uppercase, then return to the previous mode.")
{
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    Range range = get_range(&view);
    int32_t size = range.max - range.min;
    if (size <= app->memory_size){
        char *mem = (char*)app->memory;
        
        buffer_read_range(app, &buffer, range.min, range.max, mem);
        to_upper_cc(mem, mem);
        buffer_replace_range(app, &buffer, range.min, range.max, mem, size);
        view_set_cursor(app, &view, seek_pos(range.max), true);
    }
    
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_case_lower)
CUSTOM_DOC("Converts all ascii text in the range between the cursor and the mark to lowercase, then return to the previous mode.")
{
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    Range range = get_range(&view);
    int32_t size = range.max - range.min;
    if (size <= app->memory_size){
        char *mem = (char*)app->memory;
        
        buffer_read_range(app, &buffer, range.min, range.max, mem);
        to_lower_cc(mem, mem);
        buffer_replace_range(app, &buffer, range.min, range.max, mem, size);
        view_set_cursor(app, &view, seek_pos(range.max), true);
    }
    
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_case_camel)
CUSTOM_DOC("Converts all ascii text in the range between the cursor and the mark to camelcase then return to the previous mode.")
{
    uint32_t access = AccessOpen;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    Range range = get_range(&view);
    int32_t size = range.max - range.min;
    if (size <= app->memory_size){
        char *mem = (char*)app->memory;
        
        buffer_read_range(app, &buffer, range.min, range.max, mem);
        to_camel_cc(mem, mem);
        buffer_replace_range(app, &buffer, range.min, range.max, mem, size);
        view_set_cursor(app, &view, seek_pos(range.max), true);
    }
    
    nj_activate_previous_mode(app);
}

CUSTOM_COMMAND_SIG(nj_chord_case_upper_token_or_word)
CUSTOM_DOC("Selects the token or word under the cursor and converts all ascii text to uppercase and return to the previous mode.")
{
    uint32_t access = AccessProtected;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    Range range = nj_get_range_of_token_or_word(app);
    view_set_mark(app, &view, seek_pos(range.min));
    view_set_cursor(app, &view, seek_pos(range.max), 1);
    
    nj_chord_case_upper(app);
}
CUSTOM_COMMAND_SIG(nj_chord_case_lower_token_or_word)
CUSTOM_DOC("Selects the token or word under the cursor and converts all ascii text to lowercase and return to the previous mode.")
{
    uint32_t access = AccessProtected;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    Range range = nj_get_range_of_token_or_word(app);
    view_set_mark(app, &view, seek_pos(range.min));
    view_set_cursor(app, &view, seek_pos(range.max), 1);
    
    nj_chord_case_lower(app);
}
CUSTOM_COMMAND_SIG(nj_chord_case_camel_token_or_word)
CUSTOM_DOC("Selects the token or word under the cursor and converts all ascii text to camelcase and return to the previous mode.")
{
    uint32_t access = AccessProtected;
    View_Summary view = get_active_view(app, access);
    Buffer_Summary buffer = get_buffer(app, view.buffer_id, access);
    
    Range range = nj_get_range_of_token_or_word(app);
    view_set_mark(app, &view, seek_pos(range.min));
    view_set_cursor(app, &view, seek_pos(range.max), 1);
    
    nj_chord_case_camel(app);
}

#endif // _MODE_CHORD_CASE_CPP
