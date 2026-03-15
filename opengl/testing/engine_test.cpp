#include "engine.hpp"

int main()
{
    WindowManager.Start();
    WindowManager.CreateWindow("main", 800, 600, "My Engine");
    WindowManager.UseWindow("main");

    while (WindowManager.IsOpen("main"))
    {
        WindowManager.Update("main");
        WindowManager.Manage();
    }

    WindowManager.StopWindow("main");
    WindowManager.Stop();
    return 0;
}