#include <glad/glad.h> // GLAD must be included before GLFW
#include <GLFW/glfw3.h>
#include <iostream>

// we are working on vertex data
// vertex data can consist of multiple vertices
// and each vertices can have multiple vertice attributes
// in our example , for simplicity , we just have two attributes
// one of them is the vertex shader , which is basicly a 3d vertex position
// the other one , is called fragmentshader which is just an attribute for
// coloring the rendered pixel
// so the shader attribute draws the skeleton , and the fragment attribute
// colors the skeleton

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    //  if the escape key pressed on the specified window context , close that window
    //  returns glfw release if no key was pressed , ???
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        // this function changes the propery of the windowshouldclose
        // which causes the app to be closed because we have a while loop
        // that checks this property and closes the window if its true
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //-------------------------------Window Creation-------------------------------// 
    GLFWwindow* window = glfwCreateWindow(800, 600, "Learn Opengl", NULL, NULL);
    
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    // the current thread will use the  specified window
    // note that only a single window can be ran on a single thread
    // therefore if we are going to work with multiple windows
    // we need to use multithreading stuff and be careful
    glfwMakeContextCurrent(window);
    
    // we inject the address of our current context (which is the current window
    // which is ran on the main thread , in this case the window we set
    // we should do this because glad needs a pointer to the current window
    // when the specified window is resized => call the specified function
    // which in our case calls a function which executes glViewport to resize
    // this is called a callback function , we have a lot these to work with
    glfwSetFramebufferSizeCallback(window , framebuffer_size_callback);    
    
    //-------------------------------GLad Loading Opengl Function Pointer-------------------------------//
    // the address of the window context is os realted
    // which is why there is a function for it
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //-------------------------------Building and Compiling Shaders  -------------------------------//
    unsigned int vertexShader;
    // create a shader object of type GL_VERTEX_SHADER
    // note that glcreateshader returns a unsigned id for the shader object
    // returns 0 if there was an error
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    // this will replace the data in vertexshadersource into our shader object
    // which is called vertex Shader and was of type GL_BUFFER_SHADER
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // now we can compile the shader because it actually has some data
    glCompileShader(vertexShader);
    
    int success;
    char infolog[512];
    // a shader can have many status
    // here we check the GL_COMPILE_STATUS , and copy its
    // result into the success variable
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        // specifies the size of the buffer which is going to store
        // the compilaion log for the shader
        // in this case , its saying hey we want to store the log into a
        // 512 byte array , like infolog
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
    }
    
    unsigned int FragmentShader;
    // create the empty shader , of the specified type which in this case
    // its a fragment shader for coloring
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // fill the fragment shader object with the data provided in the
    // fragmentShaderSource
    
    glShaderSource(FragmentShader , 1 , &fragmentShaderSource, NULL);
    glCompileShader(FragmentShader);
    
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    // this is how we add the shaders to the pipleline
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, FragmentShader);
    // after defining the order of shaders , we link all of them into a single
    // Program , which we call shader program (its like we create the pipeline)
    glLinkProgram(shaderProgram);
    
    // we check the link status to see if the shaders have been linked
    // properly
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            // same principle as described before
            glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
        }
    
    // delete the shaders after they have been linked to the program
    glDeleteShader(vertexShader);
    glDeleteShader(FragmentShader);
    
    //---------------------------Setup the vertex Data and Buffers------------------------------//
    
    // our input is a collection of vertices which is called vertex data
    // each vertices could have many vertice attributes
    // these inputs will go through many steps before being rendered as pixel
    
    // we want to store this vertex data on our gpu , if doesnt go out of memory ofc
    // vertex buffer object is a buffer that stores this data
    // we rather have as much memory stored in the gpu , this allows vertex shader
    // which is the main step in pipeline to almost instantly use that data
    
    // vertex data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // binding the VAO object
    glBindVertexArray(VAO);
    
    
    // each buffer has a different type
    // the type of our vertex data is GL_ARRAY_BUFFER
    // so we also need to set the type of the buffer to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // so far we he have configured our buffer object and its ready to
    // store our vertex data
    // we call this function and pass neccessery stuff into it , then
    // the data will be copied into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // we then gotta connect the vertex data to vertex shader
    // we need to configure opengl so it can do that
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //---------------------------Rendering Loop------------------------------//
    
    // when the windowshouldclose property of the specified window
    // is false => keep running the render loop
    // render loop can be called as frame => every iteration of this loop called a frame
    while(!glfwWindowShouldClose(window)) {
        // this is the function we define , it keeps checking if the
        // escape key was pressed , and if it was pressed then it would change
        // the windowshouldclose property of the window , which would cause the app
        // to be closed in the next iteration
        processInput(window);
        
        // this is a state setter function => sets the color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // this is a state using color => actually uses the state
        // in this case it will clear the color and set the color that we
        // wrote in glClearColor
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); //no need to bind it everytime since we only have a single vao
        
        
        // double buffering : we have a back buffer , and front buffer
        // the front buffer waits for back buffer to be completely ready
        // then the front buffer will render whats in the back buffer
        // this reduces screen tearing but => u get shit ton of input lag
        glfwSwapBuffers(window);
        // we keep calling this function in a loop because everytime its exectued
        // the callbacks are going to be executed as well
        // which allows our window to be active and ready for input and other changes
        glfwPollEvents();    
    }   
    
    
    
    
    glfwTerminate();    
    return 0;
}
