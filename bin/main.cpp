#include "sources/rend/model/model_loader.hpp"
#include "sources/window/window.hpp"

using namespace ars_graphics;

int
main()
{
    ModelManager manager;

    ModelLoader loader;

    std::vector<std::string> paths = {
        "C:/Users/User/source/repos/arsrender_lib/models/"
        "AntiqueCamera.glb",
        "C:/Users/User/source/repos/arsrender_lib/models/"
        "BrainStem.glb"};

    loader.load(paths, manager);

    return 0;
}
