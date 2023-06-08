const char* vertexShaderSource = R"(
    #version 330 core
    
    in vec3 pos;
    
    void main(){
        gl_Position = vec4(pos.xyz, 1.0);
    }
)";

// Ref: https://www.shadertoy.com/view/4sXyDr
const char* fragmentShaderSource = R"(
    #version 330 core
    #define CLAMP

    precision mediump float;

    layout(pixel_center_integer) in vec4 gl_FragCoord;
    out vec4 frag_color;

    float distanceBezier(vec2 p, vec2 P0, vec2 P1, vec2 P2, vec2 P3)
    {
        // Cubic Bezier curve: 
        //
        //     B(t) = (1-t)^3 P0 + (1-t)^2 t P1 + (1-t) t^2 P2 + t^3 P3
        //
        // or
        //
        //     B(t) = A t^3 + B t^2 + C t + D
        //
        // where
        //
        //     A = -P0 + 3 P1 - 3 P2 + P3
        // 	   B = 3 (P0 - 2 P1 + P2)
        //     C = 3 (P1 - P0)
        //     D = P0
        
        vec2 A = -P0 + 3.0*P1 - 3.0*P2 + P3;
        vec2 B = 3.0*(P0 - 2.0*P1 + P2);
        vec2 C = 3.0*(P1 - P0);
        vec2 D = P0;
        
        // Goal:
        //
        //     "find t such that d^2 = ||B(t) - p||^2 is minimized"
        //
        // i.e. the find t such that
        //
        //     f(t) = d/dt dot(B(t) - p, B(t) - p) = 0
        // 
        // and hope that is a global minima
        
        // Expanding the above gives a 5th degree polynomial:
        //
        //     f(t) = a5 t^5 + a4 t^4 + a3 t^3 + a2 t^2 + a1 t + a0
        //
        // where 
        //
        //     a5 = 6 A A
        //     a4 = 10 A B
        //     a3 = 8 A C + 4 B B
        //     a2 = 6 A D' + 6 B C
        //     a1 = 4 B D' + 2 C C
        //     a0 = 2 C D'
        //
        // where
        // 
        //     D' = D - p
        
        float a5 = 6.0*dot(A,A);
        float a4 = 10.0*dot(A,B);
        float a3 = 8.0*dot(A,C) + 4.0*dot(B,B);
        float a2 = 6.0*dot(A,D-p) + 6.0*dot(B,C);
        float a1 = 4.0*dot(B,D-p) + 2.0*dot(C,C);
        float a0 = 2.0*dot(C,D-p);
        
        // calculate distances to the control points
        float d0 = length(p-P0);
        float d1 = length(p-P1);
        float d2 = length(p-P2);
        float d3 = length(p-P3);
        float d = min(d0, min(d1, min(d2,d3)));
        
        
        // Use the Newton-Raphson method to find a local minima, i.e. iterate:
        //
        //     t_{n+1} = t_n - f(t_n)/f'(t_n)
        //
        // until convergence is reached
        
        // Choose initial value of t
        float t;
        if (abs(d3 - d) < 1.0e-5)
            t = 1.0;
        else if (abs(d0 - d) < 1.0e-5)
            t = 0.0;
        else
            t = 0.5;
            
        // iterate
        for (int i = 0; i < 10; i++) {
            float t2 = t*t;
            float t3 = t2*t;
            float t4 = t3*t;
            float t5 = t4*t;
            
            float f = a5*t5 + a4*t4 + a3*t3 + a2*t2 + a1*t + a0;
            float df = 5.0*a5*t4 + 4.0*a4*t3 + 3.0*a3*t2 + 2.0*a2*t + a1;
            
            t = t - f/df;
        }
        
        // clamp to edge of bezier segment
    #ifdef CLAMP
        t = clamp(t, 0.0, 1.0);
    #endif
        
        // get the point on the curve
        vec2 P = A*t*t*t + B*t*t + C*t + D;
            
        // return distance to the point on the curve
        if (d < 0.1) { 
            // to see the control points
            return -1.0;
        } else {
            // Newton iteration converge to local minima that is not the global for some values
            // return length(p-P); 
            
            // Taking the union with the end point distances
            // seem to fix the issue for most "nice" control points
            // (since NR can only find interior points?)
            return min(length(p-P), min(d0, d3)); 
        }
    }

    void main(){
        vec2 P0 = vec2(0.0,0.0);
        vec2 P1 = vec2(200.0,300.0);
        vec2 P2 = vec2(400.0,300.0);
        vec2 P3 = vec2(600.0,0.0);
        vec2 B = vec2(gl_FragCoord.x, gl_FragCoord.y);

        float d = distanceBezier(B,P0,P1,P2,P3);

        if (d == -1){
            frag_color = vec4(1.0,0.0,0.0,1.0);
        }
        else if (d <= 10){
            frag_color = vec4(0.2,1.0,0.2,1.0);
        }
        else {
            frag_color = vec4(0.0,0.0,0.0,1.0);
        }

    }
)";