Imgui Framework

To create a new project, create a class that inherits from 'Project', located in project.hpp
To add the project to the application, add it to the 'projects' vector in Application::init_projects() in application.cpp

For a simple example project that draws to the main drawable background, sets up a basic menu bar, and basic editor window, please look at project_example.hpp and project_example.cpp
