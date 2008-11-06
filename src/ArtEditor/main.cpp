#include "editor.h"

int
main (int argc, char *argv[])
{

    Main kit (argc, argv);
    
    Editor::GetRef ().run();
    Editor::CleanUp();

    return 0;
}
