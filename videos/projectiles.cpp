#include "headers/main.hpp"
#include "headers/shapes.hpp"


int main()
{
    float width = 1920.0f;
    float height = 1080.0f;
    int duration = 60;
    int fps = 60;
    int totalFrames = duration * fps;

    
    InitializeHeadlessRendering((int)width, (int)height);
    //InitializeGLFW(4, 5);
    //CreateWindow((int) width, (int) height, "Projctiles");
    InitializeGLEW();

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Hardware GPU: " << renderer << std::endl;
    std::cout << "GPU Vendor: " << vendor << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;
    std::cout << "-----------------------------------" << std::endl;


    
    unsigned char* buffer = new unsigned char[(int)width * (int)height * 3];

    const char* cmd = "ffmpeg -y -f rawvideo -s 1920x1080 -pix_fmt rgb24 -r 60 -i - -vf vflip -c:v libx264 -preset fast -pix_fmt yuv420p output.mp4";
    FILE* ffmpeg = popen(cmd, "w"); 

    if (!ffmpeg) {
        std::cout << "Could not open FFmpeg! Make sure it is installed and in your system PATH." << std::endl;
        return -1;
    }

    CreateVertShader("shaders/vertexShader.vert");
    CreateFragShader("shaders/frag_simple.frag");

    CreateProgram();
    UseProgram(mainProgram);

    
    std::vector<float> sphereVertices = GenerateSphereVertices(0.5f, 100, 100, {1.0f, 0.5f, 0.327f});
    std::vector<unsigned int> sphereIndices = GenerateSphereIndices(100, 100);

    std::vector<float> planeVertices = GeneratePlaneVertices();
    std::vector<unsigned int> planeIndices = GeneratePlaneIndices();
    
    unsigned int sphereVAO, sphereVBO, sphereEBO;
    unsigned int planeVAO, planeVBO, planeEBO;

    
    // generate the vertex array object and buffer objects
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_DYNAMIC_DRAW);



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

    // tell opengl what it the vertex and what is each part of it
    // the first part is the first 3 numbers position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(10 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeEBO);

    glBindVertexArray(planeVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * sizeof(float), planeVertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, planeIndices.size() * sizeof(unsigned int), planeIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(10 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);


    glm::vec3 cameraPosition(0.0f, 1.0f, 3.0f);
    // view matrix literally too lazy to move then move the world around you
    glm::mat4 view = glm::lookAt(
    cameraPosition,
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

    // perspective matrix some complciated ass math
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), width/height, 0.1f, 100.0f);


    // model matrix that doesn't do anything right now
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 sphereModel;
    glm::mat3 sphere_normal; //glm::transpose(glm::inverse(glm::mat3(model)));
    glm::mat4 planeModel;
    glm::mat3 plane_normal;

    unsigned int model_uniform = glGetUniformLocation(mainProgram, "model");
    unsigned int view_uniform = glGetUniformLocation(mainProgram, "view");
    unsigned int perspective_uniform = glGetUniformLocation(mainProgram, "perspective");
    unsigned int normal_matrix_uniform = glGetUniformLocation(mainProgram, "normalMatrix");
    unsigned int light_pos_uniform = glGetUniformLocation(mainProgram, "lightPos");
    unsigned int camera_pos_uniform = glGetUniformLocation(mainProgram, "cameraPos");
    unsigned int light_color_uniform = glGetUniformLocation(mainProgram, "lightColor");

    glm::vec3 lightPosition(1.2f, 1.0f, 0.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    glm::vec3 spherePos(0.0f, 0.0f, 0.0f);
    glm::vec3 planePos(0.0f, 0.0f, 0.0f);
    glm::vec3 x_axis(1.0f, 1.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);

    // physics variables
    float omega = 0.1f; // radians per second
    float theta = 0.0f; // radians

    // time
    //float frame_time = glfwGetTime();
    //float current_time;
    float dt = 1.0f/60.0f;

    for(int i = 0; i < totalFrames; i++)
    //while (!glfwWindowShouldClose(window)) 
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //time
        //current_time = glfwGetTime();
        //dt = current_time - frame_time;
        //frame_time = current_time;

        // setting up the unifomrs
        UseProgram(mainProgram);
        glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(perspective_uniform, 1, GL_FALSE, glm::value_ptr(perspective));
        glUniform3fv(light_pos_uniform, 1, glm::value_ptr(lightPosition));
        glUniform3fv(light_color_uniform, 1, glm::value_ptr(lightColor));
        glUniform3fv(camera_pos_uniform, 1, glm::value_ptr(cameraPosition));

        // physics for sphere
        theta += omega * dt;
        spherePos = glm::vec3(0.0f, 2.0f * sinf(theta), 0.0f);
        sphereModel = glm::translate(model, spherePos);
        sphere_normal = glm::transpose(glm::inverse(glm::mat3(sphereModel)));
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(sphereModel));
        glUniformMatrix3fv(normal_matrix_uniform, 1, GL_FALSE, glm::value_ptr(sphere_normal));

        //drawing sphere
        glBindVertexArray(sphereVAO);
        
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

        // physics for plane
        
        planeModel = glm::rotate(model, theta, glm::vec3(1.0f, 0.0f, 0.0f));
        plane_normal = glm::transpose(glm::inverse(glm::mat3(planeModel)));
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(planeModel));
        glUniformMatrix3fv(normal_matrix_uniform, 1, GL_FALSE, glm::value_ptr(plane_normal));
        
        // drawing plane
        glBindVertexArray(planeVAO);
        
        glDrawElements(GL_TRIANGLES, planeIndices.size(), GL_UNSIGNED_INT, 0);

        // THE MAGIC: Read the pixels from the GPU back to the CPU
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

        // Write those raw bytes directly into the FFmpeg pipe
        fwrite(buffer, sizeof(unsigned char), width * height * 3, ffmpeg);

        // Print progress to the console
        if (i % 60 == 0) {
            std::cout << "Rendered " << (i / 60) << " seconds..." << std::endl;
        }
        
        //glfwSwapBuffers(window);
        //glfwPollEvents();
    }

    pclose(ffmpeg); // Tells FFmpeg "I'm done sending data, finish making the MP4"
    delete[] buffer;

    std::cout << "Render Complete! Check output.mp4" << std::endl;

    
    
    
    

    glfwTerminate();
    return 0;
    
}
