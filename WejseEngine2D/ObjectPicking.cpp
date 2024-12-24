//#include "ObjectPicking.h"
//#include "WejseRenderer.h"
//#include "GLFW/glfw3.h"
//#include "glad/glad.h"
//
//GLuint fbo, colorTexture, depthRenderbuffer;
//EntityManager& manager = EntityManager::GetInstance();
////std::unique_ptr<Shader> ObjectPicking = std::make_unique<Shader>("shader/ObjectPicker.vs", "shader/ObjectPicker.fs");
//
//
//void InitialiseEntityPicking()
//{
//    // Create a framebuffer
//    glGenFramebuffers(1, &fbo);
//    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//
//    // Create a texture to store the object IDs (as colors)
//    glGenTextures(1, &colorTexture);
//    glBindTexture(GL_TEXTURE_2D, colorTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenWidth, Screenheight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
//
//    // Create a renderbuffer for depth
//    glGenRenderbuffers(1, &depthRenderbuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ScreenWidth, Screenheight);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
//
//    // Check framebuffer completeness
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
//        // Handle the error appropriately, e.g., return or throw an exception
//        return;
//    }
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer
//}
//
//void EncodeEntityID(Entity ent)
//{
//
//
//    int id = ent.getId();
//    float r = ((id >> 16) & 0xFF) / 255.0f; // Red component
//    float g = ((id >> 8) & 0xFF) / 255.0f;  // Green component
//    float b = (id & 0xFF) / 255.0f;         // Blue component
//
//    //// Make sure the shader is bound
//    //ObjectPicking->use(); // Activate the shader
//
//
//    //ObjectPicking->setMat4("model", ent.getComponent<TransformComponent>()->transform); // Pass the color to the shader
//    //ObjectPicking->setMat4("projection", glm::ortho(-ScreenWidth / 2.0f, ScreenWidth / 2.0f, -Screenheight / 2.0f, Screenheight / 2.0f, -1.0f, 1.0f)); // Pass the color to the shader
//    //ObjectPicking->setMat4("view", glm::mat4(1.0)); // Pass the color to the shader
//    //ObjectPicking->setVec3("pickColor", glm::vec3(r, g, b)); // Pass the color to the shader 
//}
//
//void renderSceneWithPickingShader(Entity entity) {
//    glBindFramebuffer(GL_FRAMEBUFFER, fbo);  // Bind the framebuffer
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the framebuffer
//
//
//    EncodeEntityID(entity); // Encode the object ID as a color
//    
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind framebuffer
//}
//
//int DecodeEntityID(unsigned char pixel[3])
//{
//    int id = (pixel[0] << 16) | (pixel[1] << 8) | pixel[2];
//    return id;
//}
//
//
//Entity getObjectAtMousePosition(std::vector<Entity>& entities) {
//    double mouseX, mouseY;
//    glfwGetCursorPos(window, &mouseX, &mouseY);
//
//    // Convert to framebuffer coordinates (In OpenGL, origin is at the bottom-left)
//    int fbX = static_cast<int>(mouseX);
//    int fbY = Screenheight - static_cast<int>(mouseY); // Invert Y axis
//
//    unsigned char pixel[3]; // Store RGB value from the framebuffer
//    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//    glReadPixels(fbX, fbY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel); // Read the pixel color
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    int id = DecodeEntityID(pixel); // Decode object ID from the color
//    return manager.FindEntity(id); // Find and return the entity with this ID
//}