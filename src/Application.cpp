
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include<iostream>
#include<stdlib.h>
#include<glew.h>
#include<GLFW/glfw3.h>
#include<memory>
#include<fstream>
#include<string>
#include<sstream>


static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
}
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
};

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }

        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(),ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
 unsigned int id =    glCreateShader(type);
 const char* src = source.c_str();
 //the shader id, how many souce code are we specifying,mem addr of the source, length 
 glShaderSource(id, 1, &src, nullptr);

 glCompileShader(id);

 //Error handling

 //quering glCompileShader for any errors since it doesn't return any results after compiling
 int result;
 //get info from shader
 glGetShaderiv(id, GL_COMPILE_STATUS, &result);// i = id, v = vector
 if (result == GL_FALSE)
 {
     //get error message
     int length;
     glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
     //allocating mem on the stack
     char* message = (char*)_malloca(length * sizeof(char));
     //getting the info from the shader id
     glGetShaderInfoLog(id, length, &length, message);
     std::cout << "Failed to compile "<<(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader! " << std::endl;
     std::cout << message << std::endl;

     glDeleteShader(id);
     return 0;
 }

 return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{

    // note GLuint  == unsigned int
    unsigned int program = glCreateProgram(); //init process
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

//attach and link both shaders into one program

    glAttachShader(program, vs); 
    glAttachShader(program, fs);

    glLinkProgram(program); //linking
    glValidateProgram(program);

    // same as deleting the intermediate file like obj file from c++ since  we have and exe file
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main(int agr, char** argv)
{

    

        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        glewInit();
        
        /*
        float positions[]{
            -0.5f, -0.5f, 
             0.0f, 0.5f,
             0.5f, -0.5f
        };
        */

        //Position for drawing a square
        float positions[] = {
            -0.5f,-0.5f,
             0.5f, -0.5f,
             0.5f,0.5f,
            -0.5f, 0.5f,
        };
        //index buffer; reusing existing data
        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        //  vertex  buffer, memory found in the vram

        unsigned int buffer;
        glGenBuffers(1, &buffer); //number of buffers and the ID

        glBindBuffer(GL_ARRAY_BUFFER, buffer); //binding  the bufffer with type  and the buffer itsself


        //filling the buffer with data And its usage
       // glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); 

        //glBufferData for square
        glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

        //interpreting the buffer attribute ie the layout of the memory
        // index of first attrib,size(HOW MANY VERTEX 2D), type, norm, stride(SIZE TO THE NEXT VERTEX), pointer to the next attribute: in our case we have just position as the only attribute 
        glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, 2 *sizeof(float),0 ); 

        glEnableVertexAttribArray(0); //enable the attrib in the buffer and the first index


        //index buffer object for square
        //sending the index to the gpu and rendering them
        unsigned int ibo;
        glGenBuffers(1, &ibo); //number of ibo and the ID

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //binding  the ibo with type  and the buffer itsself
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 *  sizeof(unsigned int),indices, GL_STATIC_DRAW);



        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        

        unsigned int shader = CreateShader(source.VertexSource,source.FragmentSource);
        glUseProgram(shader);


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            GLClearError();
           
    /*
   
    LEGACY OPENGL CODE
            glBegin(GL_TRIANGLES);

            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.0f, 0.5f);
            glVertex2f(0.5f, -0.5f);

            glEnd();

     */
            

/*
   sharder: code that run on the GPU
 vertex and fragment shaders

 vertex shaders get call for each vertex we are rendering, in our case it will get call 3 times
 is to tell opengl where u want that vertex to be on your window

 fragment shaders,  is going to run once for each pixel that will be rasterise. ie filling in each pixel with a color-

 OTHER SHADERS
 geometry shaders
 tessellation shaders
 compute shaders

*/
        //   glDrawArrays(GL_TRIANGLES,0,3);
            
           //glDraw for square, using 6 vertecis

           
           glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
           GLCheckError();
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        std::cout << glGetString(GL_VERSION) << std::endl;

        //cleaning 
        glDeleteProgram(shader);

        glfwTerminate();
      
       
      //  std::cin.get();

	return EXIT_SUCCESS;
}