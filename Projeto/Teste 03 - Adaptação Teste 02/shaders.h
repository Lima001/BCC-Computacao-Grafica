const char* vertexShaderSource = R"(
    #version 330 core
    
    in vec3 pos;
    
    void main(){
        gl_Position = vec4(pos.xyz, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core

    #define MAX_ITERATIONS 100                  // Iteration limit for Newton's root finding method
    #define B_COLOR vec4(0.0,0.0,0.0,1.0)       // Background color
    #define P_COLOR vec4(1.0,0.0,0.0,1.0)       // Bézier curve control point color
    #define L_COLOR vec4(0.3,1.0,0.3,1.0)       // Bézier curve line color
    #define L_WIDTH 1.0                         // Line thickness
    #define P_WIDTH 5.0                         // Point radius

    precision mediump float;

    layout(pixel_center_integer) in vec4 gl_FragCoord;
    out vec4 frag_color;

    // Control points definition (in screen coordinates)
    uniform vec2 p0 = vec2(0.0, 0.0);
    uniform vec2 p1 = vec2(200.0, 300.0);
    uniform vec2 p2 = vec2(400.0, 300.0);
    uniform vec2 p3 = vec2(600.0, 0.0);

    // Fragment (X,Y) position. Used to draw the bézier curve
    vec2 p = vec2(gl_FragCoord.xy);

    float distBezier(){
        // Returns the (local) minimum distance between the point 'p' to the bézier curve defined
        // by p0, p1, p2 and p3.
        //
        // The calculation is done by trying to minimize the squared difference between the given
        // cubic bézier curve P(t) to a generic point B. Mathematically, we have:
        //
        // P(t) = w + x*t + y*t^2 + z*t^3,  
        // 
        // where w,x,y and z are the coefficients of the curve - obtained by the cubic bézier definition -
        // and are given by:
        //
        // w = p0
        // x = -3*p0 + 3*p1
        // y = 3*p0 - 6*p1 + 3*p2
        // z = -p0 + 3*p1 - 3*p2 + p3
        //
        // d/dt ((P(t) - B)^2) = 0
        //
        // Which gives us the following five-degree polynomium:
        //
        // c0 + c1*t + c2*t^2 + c3*t^3 + c4*t^4 + c5*t^5
        // 
        // where c0,c1,c2,c3,c4 and c5 are the coefficients of the polynomium and are given by:
        //
        // c0 = 2*x*(w-B)
        // c1 = 4*y*(w-B) + 2*x^2
        // c2 = 6*z*(w-B) + 6*x*y
        // c3 = 8*x*z + 4*y^2
        // c4 = 10*y*z
        // c5 = 6*z^2
        //
        // and its roots represent the minimun 't' that minimizes the equation uppermentioned.
        //
        // A root can be then found by Newton's numerical method for root finding. But 
        // keep in mind that there is no guarantee the root found is a global minimum. It might be
        // just a local spot;

        // Bézier curve coefficients
        vec2 w = p0;
        vec2 x = -3*p0 + 3*p1;
        vec2 y = 3*p0 -6*p1 + 3*p2;
        vec2 z = -p0 + 3*p1 -3*p2 +p3;

        // 5-degree polynomium - objective function for minimization - give us the 't' parameter
        // for the bézier curve that minimizes the distance between a point 'p' and the curve
        float c0 = 2.0 * dot(x,(w-p));
        float c1 = 4.0 * dot(y,(w-p)) + 2.0 * dot(x,x);
        float c2 = 6.0 * dot(z,(w-p)) + 6.0 * dot(x,y);
        float c3 = 8.0 * dot(x,z) + 4.0 * dot(y,y);
        float c4 = 10.0 * dot(y,z);
        float c5 = 6.0 * dot(z,z);

        // Newton's method for root finding
        float t = 0.5;                      // Initial guess
        float t2, t3, t4, t5, f, df;

        for (int i=0; i<MAX_ITERATIONS; i++){
            // Evaluate the values for parameter t for the coefficients
            float t2 = t*t; 
            float t3 = t2*t;
            float t4 = t3*t;
            float t5 = t4*t;
            
            // Evaluate the polynomium on t
            f = c5*t5 + c4*t4 + c3*t3 + c2*t2 + c1*t + c0;

            // Evaluate its derivative in respect to t, as demanded by the method.
            // (P.S - just derive f and you have df)
            df = 5.0*c5*t4 + 4.0*c4*t3 + 3.0*c3*t2 + 2.0*c2*t + c1;

            // Updating the guess
            t = t - f/df;
        }

        // Parameter 't' has to be in the interval [0,1]
        t = clamp(t,0.0,1.0);

        // Evaluatinf the point in the bézier curve with the obtained 't'.
        // This way we get the curve closest point to 'p' (at least, a local
        // solution)
        vec2 pB = w + x*t + y*t*t + z*t*t*t;

        return distance(p,pB);
    }

    void main(){ 

        // Check if the screen pixel point is close to a control point. If so, draw it as a control point pixel
        if (distance(p,p0) <= P_WIDTH || distance(p,p1) <= P_WIDTH || distance(p,p2) <= P_WIDTH || distance(p,p3) <= P_WIDTH){
            frag_color = P_COLOR;
        }
        // Check if the screen pixel point is close to a point in the curve. If so, draw it as a curve point pixel
        else if (distBezier() <= L_WIDTH){
            frag_color = L_COLOR;
        }
        // Otherwise, the point should be considered as background
        else {
            frag_color = B_COLOR;
        }

    }
)";