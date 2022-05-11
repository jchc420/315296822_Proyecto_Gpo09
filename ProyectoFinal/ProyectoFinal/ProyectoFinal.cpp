// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void Animacion(); //funci�n para la animaci�n compleja de la bicicleta
                   // y simple de las ruedas y pedales
void AnimacionP(); //funci�n para la animaci�n simple de la puerta
void AnimacionOU(); //funci�n para la animaci�n simple de la vida extra


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f; //VARIABLE DE ROTACI�N UTILIZADA PARA LAS LLANTAS DE LA BICICLETA
float rotB = 0.0f; //VARIABLE DE ROTACI�N UTILIZADA PARA EL FRAME DE LA 
                   //BICICLETA CON MODELADO JER�RQUICO
float animx = 0.0f; //VARIABLE DE TRASLACI�N PARA EL EJE X
float animz = 0.0f; //VARIABLE DE TRASLACI�N PARA EL EJE Z
float rot2 = 0.0f; //VARIABLE DE ROTACI�N PARA LA PUERTA
float oneupRot = 0.0f; //VARIABLE DE ROTACI�N PARA LA VIDA EXTRA DE SCOTT PILGRIM
bool anim = false; //variable de estado para la animaci�n de la bicicleta
bool izq = true; //variable de estado para la animaci�n de la bicicleta
bool der = false; //variable de estado para la animaci�n de la bicicleta
bool der2 = false; //variable de estado para la animaci�n de la bicicleta
bool izq2 = false; //variable de estado para la animaci�n de la bicicleta
bool abierto = false; //variable de estado para la animaci�n de la puerta
bool cerrado = true; //variable de estado para la animaci�n de la puerta
bool animP = false; //variable de estado para la animaci�n de la vida extra
bool animOU = false; //variable de estado para la animaci�n de la vida extra

float tiempo; //variable utilizada para la animaci�n con el shader anim2.vs


int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Proyecto Final - 315296822", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shaders/modelLoading.vs", "Shaders/modelLoading.frag" );
    Shader lampshader( "Shaders/lamp.vs", "Shaders/lamp.frag" );
    Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
    Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
    Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
    Shader Anim2("Shaders/anim2.vs", "Shaders/anim2.frag");



    // Load models
    //Model Pizza((char*)"Models/Pizza/PizzaSteve.obj");
    //Model brader((char*)"Models/Pizza/brader.obj");
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    GLfloat vertices[] =
    {
        // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 

    };

    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    GLuint indices[] =
    {  // Note that we start from 0!
        0,1,3,
        1,2,3

    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //SKYBOX
    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    //SKYBOX TEXTURES
    vector<const GLchar*> faces;
	faces.push_back("SkyBox/sb_right.tga"); //Cara derecha 
	faces.push_back("SkyBox/sb_left.tga"); //Cara izquierda
	faces.push_back("SkyBox/sb_top.tga"); //Cara de arriba
	faces.push_back("SkyBox/sb_bottom.tga"); //Cara de abajo
	faces.push_back("SkyBox/sb_back.tga"); //Cara de atr�s
	faces.push_back("SkyBox/sb_front.tga"); //Cara de frente

    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

    //glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

    // Load textures
    Model estante((char*)"Models/estante/estante.obj");
    Model cama((char*)"Models/cama/cama.obj");
    Model mesita((char*)"Models/mesita/mesita.obj");
    Model tv((char*)"Models/tv/tv.obj");
    Model cuarto((char*)"Models/cuarto/cuarto.obj");
    Model piso_cuarto((char*)"Models/piso_cuarto/piso_cuarto.obj");
    Model pc((char*)"Models/pc/pc.obj");
    Model perchero((char*)"Models/perchero/perchero.obj");
    Model alfombra((char*)"Models/alfombra/alfombra.obj");
    Model fachada((char*)"Models/fachada/fachada.obj");
    Model piso((char*)"Models/piso/piso.obj");
    Model calle((char*)"Models/calle/calle.obj");
    Model faro((char*)"Models/faro/faro.obj");
    Model oneup((char*)"Models/1up/1up.obj");
    Model puerta((char*)"Models/puerta/puerta.obj");
    Model bici((char*)"Models/bici/bici.obj");
    Model bici_pedales((char*)"Models/bici/bici_pedales.obj");
    Model bici_ruedafrente((char*)"Models/bici/bici_ruedafrente.obj");
    Model bici_ruedatrasera((char*)"Models/bici/bici_ruedatrasera.obj");

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  
    image = stbi_load("images/goku.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();
        Animacion();
        AnimacionP();
        AnimacionOU();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glm::mat4 modelTemp = glm::mat4(1.0f); //TEMP
        //glDrawElements(GL_TRIANGLES, 6, GL_FLAT, 0);
        //Pizza.Draw(shader);


        /*model = glm:: mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pokeArriba.Draw(shader);

        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pokeAbajo.Draw(shader);*/
        //ESTANTE
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        estante.Draw(shader);
        //CAMA
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cama.Draw(shader);
        //MESITA
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        mesita.Draw(shader);
        //TELEVISI�N
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tv.Draw(shader);
        //PAREDES DEL CUARTO 
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cuarto.Draw(shader);
        //PISO DEL CUARTO
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        piso_cuarto.Draw(shader);
        //COMPUTADORA
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pc.Draw(shader);
        //PERCHERO
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        perchero.Draw(shader);
        //ALFOMBRA
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        alfombra.Draw(shader);
        //FACHADA
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        fachada.Draw(shader);
        //PISO DEL MUNDO
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        piso.Draw(shader);
        //CALLE
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        calle.Draw(shader);
        //FAROS DE LA CALLE
        model = glm::mat4(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        faro.Draw(shader);        
        //PUERTA
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.35f, 0.872f, -1.168f));
        model = glm::rotate(model, glm::radians(rot2), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        puerta.Draw(shader);

        //FRAME DE LA BICICLETA
        model = glm::mat4(1);
        modelTemp = model = glm::translate(model, glm::vec3(-3.752f + animx, 0.843f, -9.955f + animz));
        model = glm::rotate(model, glm::radians(rotB), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        bici.Draw(shader);
        
        //PEDALES BICICLETA
        model = glm::mat4(1);
        model = glm::rotate(modelTemp, glm::radians(rotB), glm::vec3(0.0f, 1.0f, 0.0f));    
        model = glm::translate(model, glm::vec3(-0.15f, -0.385f, 0.0f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        bici_pedales.Draw(shader);

        //RUEDA DELANTERA BICICLETA
        model = glm::mat4(1);        
        model = glm::rotate(modelTemp, glm::radians(rotB), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.739f , -0.4f, 0.0f ));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        bici_ruedafrente.Draw(shader);

        //RUEDA TRASERA BICICLETA
        model = glm::mat4(1);        
        model = glm::rotate(modelTemp, glm::radians(rotB), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-0.7f, -0.4f, 0.0f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        bici_ruedatrasera.Draw(shader);
        glBindVertexArray(0);

        //USO DE SHADER "ANIM2" para animar ONEUP 

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        Anim2.Use();
        tiempo = glfwGetTime();
        modelLoc = glGetUniformLocation(Anim2.Program, "model");
        viewLoc = glGetUniformLocation(Anim2.Program, "view");
        projLoc = glGetUniformLocation(Anim2.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(Anim2.Program, "time"), tiempo);

        //ONEUP - VIDA EXTRA
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.63f, 1.36f, 0.268f));
        model = glm::rotate(model, glm::radians(oneupRot), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(Anim2.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        oneup.Draw(Anim2);

        glBindVertexArray(0);

        /*glActiveTexture(GL_TEXTURE0);*/
        /*glBindTexture(GL_TEXTURE_2D, texture);*/
        /*lampshader.Use();*/
        //glm::mat4 model(1);
       /*glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);*/

        // Draw skybox as last
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
        SkyBoxshader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Set depth function back to default
       
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

    /*if (keys[GLFW_KEY_O])
    {
        if (rot == 0.0f) 
            anim = true;                     
        if (rot == 90.0f)
            anim2 = true;
    }

    if (anim) {
        if (rot <= 90.0f)
            rot += 0.5f;
    }

    if (anim2) {
        if (rot >= 0.0f)
            rot -= 0.5f;        
    }*/

 
        
            

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_M]) {
        anim = true;
    }

    if (keys[GLFW_KEY_N]) {
        anim = false;
    }

    if (keys[GLFW_KEY_O]) {
        animP = true;
    }

    if (keys[GLFW_KEY_X]) {
        animOU = true;
    }

    if (keys[GLFW_KEY_Z]) {
        animOU = false;
    }
}

void Animacion() {
    
    if (anim) {
        if (izq) {
            rot += 10.0f;
            rotB = 45.0f;
            animx += 0.1f;
            animz -= 0.1f;
            if (animz <= -5.5f) {
                izq = false;
                der = true;
            }

            if (animx >= 38.5f) {
                izq = false;
                der = false;
            }
        }

        if (der) {
            rot += 10.0f;
            rotB = 315.0f;
            animx += 0.1f;
            animz += 0.1f;
            if (animz >= 1.0f) {
                der = false;
                izq = true;
            }
            if (animx >= 38.5f) {
                izq = false;
                der = false;
                der2 = true;
            }
        }

        if (der2) {
            rot += 10.0f;
            rotB = 135.0f;
            animx -= 0.1f;
            animz -= 0.1f;
            if (animz <= -5.5f) {
                der2 = false;
                izq2 = true;
            }
            if (animx <= -28.0f) {
                izq2 = false;           
                der2 = false;
                izq = true;
            }
        }

        if (izq2) {
            rot += 10.0f;
            rotB = 225.0f;
            animx -= 0.1f;
            animz += 0.1f;
            if (animz >= 1.0f) {
                izq2 = false;
                der2 = true;
            }
            if (animx <= -28.0f) {
                izq2 = false;                
                der2 = false;
                izq = true;
            }
        }
    }
    
}

void AnimacionP() {
    if (animP) {
        if (cerrado) {
            rot2 += 0.9f;
            if (rot2 >= 90) {
                cerrado = false;
                abierto = true;
                animP = false;

            }
        }
        if (abierto) {
            rot2 -= 0.9f;
            if (rot2 <= 0) {
                abierto = false;
                cerrado = true;
                animP = false;
            }
        }
    }
}

void AnimacionOU() {
    if (animOU) {
        oneupRot -= 1.0f;
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

