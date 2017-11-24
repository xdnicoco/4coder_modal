# 4coder_modal
This is my customization layer of 4coder super in the journey to a modal future.
I took some elements from emacs, and some elements from vim, and added my own functions and functionality to create a highly customzied modal system.

Tested only on 4coder 2.24.

To activate this customization, support [Allen Webster on Patreon](https://www.patreon.com/mr4thdimention/) clone the repository into your 4coder directory and edit 4coder_default_bindings to the following:
```#if !defined(FCODER_DEFAULT_BINDINGS)
#define FCODER_DEFAULT_BINDINGS

#include "4coder_default_include.cpp"
#include "4coder_modal/nj_modal.cpp"
#include "4coder_modal/nj_hooks.cpp"

// NOTE(allen|a4.0.22): This no longer serves as very good example code.
// Good example code will be coming soon, but in the mean time you can go
// to 4coder_remapping_commands.cpp for examples of what binding code looks like.

#if !defined(NO_BINDING)
extern "C" int32_t
get_bindings(void *data, int32_t size){
    Bind_Helper context_ = begin_bind_helper(data, size);
    Bind_Helper *context = &context_;
    
    // set_all_default_hooks(context);
    nj_set_default_hooks(context);
#if defined(__APPLE__) && defined(__MACH__)
    mac_default_keys(context);
#else
    // default_keys(context);
    nj_keys(context);
#endif
    
    int32_t result = end_bind_helper(context);
    return(result);
}
#endif //NO_BINDING

#endif //FCODER_DEFAULT_BINDINGS
```

For further information and documentaion check the [4coder documentaion page](http://4coder.net/custom_docs.html).
