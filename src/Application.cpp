#include<iostream>
#include<stdlib.h>
#include<glew.h>
#include <GLFW/glfw3.h>



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

        //interpreting the buffer attribute
        glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, 2 *sizeof(float),0 ); // index of first attrib, size(HOW MANY VERTEX 2D), type, norm, stride(SIZE TO THE NEXT VERTEX), pointer to the next attribute

        glEnableVertexAttribArray(0); //enable the attrib in the buffer and the first index
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

*/
            glDrawArrays(GL_TRIANGLES,0,3);
            
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        std::cout << glGetString(GL_VERSION) << std::endl;
        glfwTerminate();
      
       
      //  std::cin.get();

	return EXIT_SUCCESS;
}