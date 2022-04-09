#include<iostream>
#include<stdlib.h>
#include<glew.h>
#include <GLFW/glfw3.h>
#include<memory>
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
        
        float positions[]{
            -0.5f, -0.5f, 
             0.0f, 0.5f,
             0.5f, -0.5f
        };

        //  vertext buffer, memory found in the vram

        unsigned int buffer;
        glGenBuffers(1, &buffer); //number of buffers and the ID

        glBindBuffer(GL_ARRAY_BUFFER, buffer); //binding  the bufffer with type  and the buffer itsself

        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //filling the buffer with data And its usage

        //interpreting the buffer attribute ie the layout of the memory
        // index of first attrib,size(HOW MANY VERTEX 2D), type, norm, stride(SIZE TO THE NEXT VERTEX), pointer to the next attribute: in our case we have just position as the only attribute 
        glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, 2 *sizeof(float),0 ); 

        glEnableVertexAttribArray(0); //enable the attrib in the buffer and the first index

        //writing vextor shader
        std::string vertexShader = 
            "#version 430 core \n"
            "\n"
            //locating the attribute in the vectex, in this case 0 as seen in glVertexAttribPointer
            "layout(location = 0) in vec2 position;\n" //note position have 2 dimention
            "\n"
            "void main()\n"
            "{\n"
            "gl_Position = vec4(position.xy,0.0,1.0);\n"
            
            "}\n";
        //writing frament shader
        std::string fragmentShader = 
            "#version 430 core \n"
         
            "\n"
            "void main()\n"
            "{\n"
            "gl_FragColor = vec4(1.0,0.0,0.0,1.0);\n"
            
            "}\n";

        unsigned int shader = CreateShader(vertexShader,fragmentShader);
        glUseProgram(shader);


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

        

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
           glDrawArrays(GL_TRIANGLES,0,3);
            
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