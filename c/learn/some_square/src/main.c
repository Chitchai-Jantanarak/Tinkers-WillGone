#include "platform.h"
#include "renderer.h"
#include "winsize_ctl.h"
#include "math3d.h"
#include "mesh.h"
#include "shader.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Simple function to draw a triangle using wireframe
void draw_triangle(Renderer* r, Vec4 v0, Vec4 v1, Vec4 v2, char unused_c, float brightness) {
    (void)unused_c; // Suppress unused parameter warning
    // Convert to screen coordinates
    int x0 = (int)v0.x, y0 = (int)v0.y;
    int x1 = (int)v1.x, y1 = (int)v1.y;
    int x2 = (int)v2.x, y2 = (int)v2.y;
    
    // Use brightness to determine character
    const char* ramp = " 123456789";
    int ramp_len = 10;
    int char_idx = (int)(brightness * (ramp_len - 1));
    if (char_idx < 0) char_idx = 0;
    if (char_idx >= ramp_len) char_idx = ramp_len - 1;
    char shade_char = ramp[char_idx];
    
    // Simple line drawing for triangle edges
    // Line from v0 to v1
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int x = x0, y = y0;
    int x_inc = (x0 < x1) ? 1 : -1;
    int y_inc = (y0 < y1) ? 1 : -1;
    int error = dx - dy;
    
    for (int i = 0; i <= dx + dy && i < 200; i++) {
        renderer_plot(r, x, y, (v0.z + v1.z) * 0.5f, shade_char);
        if (error > 0) {
            x += x_inc;
            error -= dy;
        } else {
            y += y_inc;
            error += dx;
        }
        if (x == x1 && y == y1) break;
    }
    
    // Line from v1 to v2
    dx = abs(x2 - x1), dy = abs(y2 - y1);
    x = x1, y = y1;
    x_inc = (x1 < x2) ? 1 : -1;
    y_inc = (y1 < y2) ? 1 : -1;
    error = dx - dy;
    
    for (int i = 0; i <= dx + dy && i < 200; i++) {
        renderer_plot(r, x, y, (v1.z + v2.z) * 0.5f, shade_char);
        if (error > 0) {
            x += x_inc;
            error -= dy;
        } else {
            y += y_inc;
            error += dx;
        }
        if (x == x2 && y == y2) break;
    }
    
    // Line from v2 to v0
    dx = abs(x0 - x2), dy = abs(y0 - y2);
    x = x2, y = y2;
    x_inc = (x2 < x0) ? 1 : -1;
    y_inc = (y2 < y0) ? 1 : -1;
    error = dx - dy;
    
    for (int i = 0; i <= dx + dy && i < 200; i++) {
        renderer_plot(r, x, y, (v2.z + v0.z) * 0.5f, shade_char);
        if (error > 0) {
            x += x_inc;
            error -= dy;
        } else {
            y += y_inc;
            error += dx;
        }
        if (x == x0 && y == y0) break;
    }
}

int main(void) {
    platform_enter_raw();
    platform_hide_cursor();
    platform_clear_screen();

    terminal_size_t ts;
    if (get_terminal_size(&ts) != 0) {
        printf("Failed to get terminal size\n");
        return 1;
    }
    
    setTerminalSize(ts.col, ts.row);
    
    Renderer* R = renderer_create(ts.row, ts.col);
    if (!R) {
        printf("Failed to create renderer\n");
        platform_show_cursor();
        platform_leave_raw();
        return 1;
    }

    // Create a smaller cube mesh
    Mesh* cube = mesh_make_cube(2.0f);
    if (!cube) {
        printf("Failed to create cube mesh\n");
        renderer_destroy(R);
        platform_show_cursor();
        platform_leave_raw();
        return 1;
    }

    // Set up shader
    const char* ramp = " .:-=+*#%@";
    ShaderLambertCfg shader_cfg = {
        .lx = 0.5773f,   // Light direction (normalized vector pointing diagonally)
        .ly = 0.5773f,
        .lz = 0.5773f,
        .ramp = ramp
    };
    Shader shader = shader_make_lambert(&shader_cfg);

    // Get mesh data
    size_t vertex_count, triangle_count, normal_count;
    const Vec3* vertices = mesh_vertices(cube, &vertex_count);
    const Tri* triangles = mesh_triangles(cube, &triangle_count);
    const Vec3* normals = mesh_normals(cube, &normal_count);

    // Perspective projection matrix with narrower FOV
    float aspect = (float)ts.col / (float)ts.row * 0.5f; // Adjust for character aspect ratio
    Mat4 projection = mat4_perspective(M_PI * 0.15f, aspect, 0.1f, 100.0f); // Narrower FOV (was 0.25f)
    
    float rotation_x = 0.0f, rotation_y = 0.0f;
    float base_speed = 0.02f;
    
    printf("Move mouse to control rotation speed in different quadrants!\n");
    printf("Press Ctrl+C to exit.\n");
    platform_sleep_ms(2000);
    
    unsigned long long last_time = platform_now_ms();
    
    // Main loop - shorter duration
    for (int frame = 0; frame < 1000; frame++) {
        unsigned long long current_time = platform_now_ms();
        // Use last_time to prevent warning
        (void)last_time; 
        last_time = current_time;
        
        // Update mouse tracker
        runMouseTracker();
        
        // Get mouse position and classify quadrant
        int mouse_x = getMouseX();
        int mouse_y = getMouseY();
        INPUT_MOUSE_POS quadrant = classifyQuadrant(mouse_x, mouse_y, ts.col, ts.row);
        
        // Calculate rotation speed multipliers based on quadrant
        float speed_x = base_speed, speed_y = base_speed;
        float factor = 1.0f + (float)(mouse_x + mouse_y) / (float)(ts.col + ts.row);
        
        switch (quadrant) {
            case TOP_LEFT:
                speed_x = base_speed * factor * 2.0f;
                speed_y = base_speed * factor * 0.5f;
                break;
            case TOP_RIGHT:
                speed_x = base_speed * factor * 0.5f;
                speed_y = base_speed * factor * 2.0f;
                break;
            case BOT_LEFT:
                speed_x = -base_speed * factor * 1.5f;
                speed_y = base_speed * factor * 1.5f;
                break;
            case BOT_RIGHT:
                speed_x = base_speed * factor * 1.5f;
                speed_y = -base_speed * factor * 1.5f;
                break;
        }
        
        // Update rotations
        rotation_x += speed_x;
        rotation_y += speed_y;
        
        // Clear renderer
        renderer_clear(R, ' ');
        
        // Create transformation matrices
        Mat4 rot_x = mat4_rx(rotation_x);
        Mat4 rot_y = mat4_ry(rotation_y);
        Mat4 rotation = mat4_mul(rot_y, rot_x);
        
        // Translation matrix to move cube further away from camera
        Mat4 translation = mat4_identity();
        translation.m[14] = -120.0f; // Move 12 units away from camera (was -8)
        
        Mat4 model_view = mat4_mul(translation, rotation);
        Mat4 mvp = mat4_mul(projection, model_view);
        
        // Transform and render triangles
        for (size_t i = 0; i < triangle_count; i++) {
            Tri tri = triangles[i];
            Vec3 v0 = vertices[tri.a];
            Vec3 v1 = vertices[tri.b];
            Vec3 v2 = vertices[tri.c];
            Vec3 normal = normals[i];
            
            // Transform vertices to homogeneous coordinates
            Vec4 h0 = vec4(v0.x, v0.y, v0.z, 1.0f);
            Vec4 h1 = vec4(v1.x, v1.y, v1.z, 1.0f);
            Vec4 h2 = vec4(v2.x, v2.y, v2.z, 1.0f);
            
            // Transform vertices
            Vec4 t0 = vec4_mul(h0, &mvp);
            Vec4 t1 = vec4_mul(h1, &mvp);
            Vec4 t2 = vec4_mul(h2, &mvp);
            
            // Perspective divide
            if (t0.w > 0.001f) { t0.x /= t0.w; t0.y /= t0.w; t0.z /= t0.w; }
            if (t1.w > 0.001f) { t1.x /= t1.w; t1.y /= t1.w; t1.z /= t1.w; }
            if (t2.w > 0.001f) { t2.x /= t2.w; t2.y /= t2.w; t2.z /= t2.w; }
            
            // Back-face culling (simple)
            Vec3 edge1 = {t1.x - t0.x, t1.y - t0.y, t1.z - t0.z};
            Vec3 edge2 = {t2.x - t0.x, t2.y - t0.y, t2.z - t0.z};
            float cross_z = edge1.x * edge2.y - edge1.y * edge2.x;
            if (cross_z < 0) continue; // Back-facing
            
            // Convert to screen coordinates with smaller scale
            t0.x = (t0.x + 1.0f) * 0.3f * ts.col + ts.col * 0.35f; // Center and scale down
            t0.y = (1.0f - t0.y) * 0.3f * ts.row + ts.row * 0.35f;
            t1.x = (t1.x + 1.0f) * 0.3f * ts.col + ts.col * 0.35f;
            t1.y = (1.0f - t1.y) * 0.3f * ts.row + ts.row * 0.35f;
            t2.x = (t2.x + 1.0f) * 0.3f * ts.col + ts.col * 0.35f;
            t2.y = (1.0f - t2.y) * 0.3f * ts.row + ts.row * 0.35f;
            
            // Calculate lighting
            float brightness = shader.brightness_face(normal, shader.user);
            
            // Draw triangle
            draw_triangle(R, t0, t1, t2, '@', brightness);
        }
        
        // Display mouse info
        const char* quad_names[] = {"TOP_LEFT", "TOP_RIGHT", "BOT_LEFT", "BOT_RIGHT"};
        renderer_plot(R, 1, 1, -1000, 'M');
        renderer_plot(R, 2, 1, -1000, 'o');
        renderer_plot(R, 3, 1, -1000, 'u');
        renderer_plot(R, 4, 1, -1000, 's');
        renderer_plot(R, 5, 1, -1000, 'e');
        renderer_plot(R, 6, 1, -1000, ':');
        
        char mouse_info[64];
        snprintf(mouse_info, sizeof(mouse_info), "(%d,%d) %s", mouse_x, mouse_y, quad_names[quadrant]);
        for (int j = 0; j < (int)strlen(mouse_info) && j < ts.col - 8; j++) {
            renderer_plot(R, 8 + j, 1, -1000, mouse_info[j]);
        }
        
        // Present frame
        renderer_present(R);
        
        // Control frame rate - faster updates
        platform_sleep_ms(10);
    }
    
    // Cleanup
    mesh_destroy(cube);
    renderer_destroy(R);
    
    // Disable mouse tracking
    printf("\033[?1003l\033[?1006l");
    fflush(stdout);
    
    platform_show_cursor();
    platform_leave_raw();
    printf("\nSpinning cube demo completed!\n");
    
    return 0;
}