/*
 nj 4coder custom hooks
*/

#if !defined(FCODER_NJ_HOOKS_CPP)
#define FCODER_NJ_HOOKS_CPP

//
// Custom Hooks
//

static void enter_mode_normal(struct Application_Links *app, int buffer_id);

static char *
nj_remove_path_from_filename(char *filename){
    int32_t len = str_size(filename);
    for(char *c = filename + len-1;
        c >= filename;
        --c)
    {
        if(*(c-1) == '\\' || *(c-1) == '/'){
            filename = c;
            break;
        }
    }
    
    return(filename);
}

static void
nj_4coder_dual_pannels(Application_Links *app, Buffer_Identifier first_buffer, Buffer_Identifier second_buffer){
    Buffer_ID first_id = buffer_identifier_to_id(app, first_buffer);
    
    // Left Panel
    View_Summary view = get_active_view(app, AccessAll);
    new_view_settings(app, &view);
    view_set_buffer(app, &view, first_id, 0);
    
#if !defined(IS_WINDOWS)
    // Right Panel
    int32_t view_width  = view.view_region.x1 - view.view_region.x0;
    int32_t view_height  = view.view_region.y1 - view.view_region.y0;
    if(view_width >= view_height){
        Buffer_ID second_id = buffer_identifier_to_id(app, second_buffer);
        
        open_panel_vsplit(app);
        View_Summary second_view = get_active_view(app, AccessAll);
        view_set_buffer(app, &second_view, second_id, 0);
    }
#endif
    
    // Restore Active to Left
    set_active_view(app, &view);
}

static void
nj_init(Application_Links *app, char **command_line_files, int32_t file_count){
    Buffer_Identifier first = buffer_identifier(literal("*scratch*"));
    Buffer_Identifier second = buffer_identifier(literal("*messages*"));
    change_theme(app, literal("Handmade Burn"));
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(normal);
    if (file_count > 0){
        char *file_1 = nj_remove_path_from_filename(command_line_files[0]);
        
        int32_t len_1 = str_size(file_1);
        first = buffer_identifier(file_1, len_1);
        
        if (file_count > 1){
            char *file_2 = nj_remove_path_from_filename(command_line_files[1]);
            
            int32_t len_2 = str_size(file_2);
            second = buffer_identifier(file_2, len_2);
        }
    }
    
    char msg[] = "init_file_test";
    print_message(app, msg, str_size(msg));
    
    
    nj_4coder_dual_pannels(app, first, second);
}

OPEN_FILE_HOOK_SIG(nj_file_settings){
    Buffer_Summary buffer = get_buffer(app, buffer_id, AccessAll);
    bool32 treat_as_code = 0;
    if(buffer.file_name && buffer.size < (16 << 20)){
        String ext = file_extension(make_string(buffer.file_name, buffer.file_name_len));
        if(match(ext, make_lit_string("cpp"))) treat_as_code = true;
        else if(match(ext, make_lit_string("h"))) treat_as_code = true;
        else if(match(ext, make_lit_string("c"))) treat_as_code = true;
        else if(match(ext, make_lit_string("hpp"))) treat_as_code = true;
    }
    
    buffer_set_setting(app, &buffer, BufferSetting_Lex, treat_as_code);
    buffer_set_setting(app, &buffer, BufferSetting_WrapLine, !treat_as_code);
    
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(normal);
    
    return(0);
}

START_HOOK_SIG(nj_start){
    named_maps = nj_named_maps_values;
    named_map_count = ArrayCount(nj_named_maps_values);
    nj_previous_mapid = mapid_normal;
    
    default_4coder_initialize(app);
    nj_init(app, files, file_count);
    
    if (automatically_load_project){
        load_project(app);
    }
    
    // no meaning for return
    return(0);
}

OPEN_FILE_HOOK_SIG(nj_new_file){
    int32_t access = AccessAll;
    Buffer_Summary buffer = get_buffer(app, buffer_id, access);
    String ext = file_extension(make_string(buffer.file_name, buffer.file_name_len));
    View_Summary view = get_active_view(app, access);
    if(match_insensitive_sc(ext, "h") || match_insensitive_sc(ext, "hpp")){
        exec_command(app, nj_include_gaurd_file);
        view_set_cursor(app, &view, seek_line_char(3, 0), 1);
    }
    
    return(0);
}

static void
nj_set_default_hooks(Bind_Helper *context){
    set_hook(context, hook_exit, default_exit);
    set_hook(context, hook_view_size_change, default_view_adjust);
    
    set_start_hook(context, nj_start);
    set_open_file_hook(context, default_file_settings);
    set_new_file_hook(context, nj_new_file);
    set_save_file_hook(context, default_file_save);
    
#if defined(FCODER_STICKY_JUMP)
    set_end_file_hook(context, end_file_close_jump_list);
#else
    set_end_file_hook(context, default_end_file);
#endif
    
    set_command_caller(context, default_command_caller);
    set_input_filter(context, default_suppress_mouse_filter);
    set_scroll_rule(context, smooth_scroll_rule);
}

#endif // FCODER_NJ_HOOKS_CPP
