/*
 nj 4coder custom hooks
*/

#if !defined(FCODER_NJ_HOOKS_CPP)
#define FCODER_NJ_HOOKS_CPP

//
// Custom Hooks
//

static void enter_mode_normal(struct Application_Links *app, int buffer_id);

CUSTOM_COMMAND_SIG(nj_set_defult_modal_color_scheme){
    // NOTE(NJ): The following color scheme is tuned to work best with the mode color scheme
    // modifications when switching modes.
    int_color color_bg                = 0xFF1F1F1F;
    int_color color_margin            = 0xFF262626;
    int_color color_margin_hover      = 0xFF333333;
    int_color color_margin_active     = 0xFF404040;
    int_color color_cursor            = 0xFFE95420;
    int_color color_highlight         = 0xFF703419;
    int_color color_mark              = 0xFF8D360d;
    int_color color_default           = 0xFFDADACA;
    int_color color_at_highlight      = 0xFFCDAA7D;
    int_color color_comment           = 0xFF7E9E7E;
    int_color color_keyword           = 0xFFF5C59b;
    int_color color_str_constant      = 0xFFCD9494;
    int_color color_int_constant      = 0xFFCAB080;
    int_color color_preproc           = 0xFF6BE0E3;
    int_color color_special_character = 0xFFFF0000;
    int_color color_ghost_character   = 0xFF5B4D3C;
    int_color color_paste             = 0xFF2DFFAB;
    int_color color_undo              = 0xFFE6644D;
    int_color color_highlight_junk    = 0xFF3A0000;
    int_color color_highlight_white   = 0xFF003A3A;
    int_color color_bar               = 0xFF30333F;
    int_color color_bar_active        = 0xFF000000;
    int_color color_base              = 0xFFAAAAAA;
    int_color color_pop1              = 0xFF03CF0C;
    int_color color_pop2              = 0xFFFF0000;
    
    Theme_Color colors[] =  {
        { Stag_Back,              color_bg},
        { Stag_Margin,            color_margin},
        { Stag_Margin_Hover,      color_margin_hover},
        { Stag_Margin_Active,     color_margin_active},
        
        { Stag_List_Item,         color_margin},
        { Stag_List_Item_Hover,   color_margin_hover},
        { Stag_List_Item_Active,  color_margin_active},
        { Stag_Cursor,            color_cursor },
        { Stag_Highlight,         color_highlight },
        { Stag_Mark,              color_mark },
        { Stag_Default,           color_default },
        { Stag_At_Cursor,         color_default },
        { Stag_At_Highlight,      color_at_highlight },
        { Stag_Comment,           color_comment },
        { Stag_Keyword,           color_keyword },
        { Stag_Str_Constant,      color_str_constant },
        { Stag_Char_Constant,     color_str_constant },
        { Stag_Int_Constant,      color_int_constant },
        { Stag_Float_Constant,    color_int_constant },
        { Stag_Bool_Constant,     color_int_constant },
        { Stag_Include,           color_str_constant },
        { Stag_Preproc,           color_preproc },
        { Stag_Special_Character, color_special_character },
        { Stag_Ghost_Character,   color_ghost_character },
        
        { Stag_Paste,             color_paste },
        
        { Stag_Undo,              color_undo },
        
        { Stag_Highlight_Junk,    color_highlight_junk },
        { Stag_Highlight_White,   color_highlight_white },
        
        { Stag_Bar,               color_bar },
        { Stag_Bar_Active,        color_bar_active },
        { Stag_Base,              color_base },
        { Stag_Pop1,              color_pop1 },
        { Stag_Pop2,              color_pop2 },
    };
    set_theme_colors(app, colors, ArrayCount(colors));
}

static void
nj_4coder_initialized_by_space(Application_Links *app, Buffer_Identifier first_buffer, Buffer_Identifier second_buffer){
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
    
    nj_set_defult_modal_color_scheme(app);
    NJ_MODE_ACTIVATE_ENTER_FUNCTION(normal);
}

static void
nj_init(Application_Links *app, char **command_line_files, int32_t file_count){
    Buffer_Identifier left  = buffer_identifier(literal("*scratch*"));
    Buffer_Identifier right = buffer_identifier(literal("*messages*"));
    
    if (file_count > 0){
        char *left_name = command_line_files[0];
        int32_t left_len = str_size(left_name);
        left = buffer_identifier(left_name, left_len);
        
        if (file_count > 1){
            char *right_name = command_line_files[1];
            int32_t right_len = str_size(right_name);
            right = buffer_identifier(right_name, right_len);
        }
    }
    
    nj_4coder_initialized_by_space(app, left, right);
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

OPEN_FILE_HOOK_SIG(nj_file_save){
    Buffer_Summary buffer = get_buffer(app, buffer_id, AccessAll);
    Assert(buffer.exists);
    
#if defined(FCODER_AUTO_INDENT_CPP)
    int32_t is_virtual = 0;
    if (automatically_indent_text_on_save && buffer_get_setting(app, &buffer, BufferSetting_VirtualWhitespace, &is_virtual)){
        if (is_virtual){
            auto_tab_whole_file_by_summary(app, &buffer);
        }
    }
#endif
    exec_command(app, clean_all_lines);
    
    // no meaning for return
    return(0);
}

static void
nj_set_default_hooks(Bind_Helper *context){
    set_hook(context, hook_exit, default_exit);
    set_hook(context, hook_view_size_change, default_view_adjust);
    
    set_start_hook(context, nj_start);
    set_open_file_hook(context, default_file_settings);
    set_new_file_hook(context, nj_new_file);
    set_save_file_hook(context, nj_file_save);
    
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
