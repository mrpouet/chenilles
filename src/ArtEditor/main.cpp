#include "editor.h"

int
main (int argc, char *argv[])
{

    Main kit (argc, argv);
    Editor & editor = Editor::GetRef ();

    editor.run ();

    return 0;
}
