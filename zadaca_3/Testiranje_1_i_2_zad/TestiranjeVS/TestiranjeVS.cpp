#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "geometry.h"
#include "OBJparser.h"
#include "Matrix.h"
#include "tgaimage.h"

int globalC;

// struktura
typedef std::vector<Vec3f> Image;

using namespace std;



struct Light
{
    Vec3f pos;
    float intenstity;

    Light(const Vec3f& pos, const float& i) : pos(pos), intenstity(i) {}
};
typedef std::vector<Light> Lights;

struct Material
{
    Vec3f diffuse_color;
    Vec2f albedo;
    float specular_coef;


    Material(const Vec2f& albedo, const Vec3f& color, float exp) : diffuse_color(color), albedo(albedo), specular_coef(exp) {}
    Material() : albedo(Vec2f(1, 0)), diffuse_color(Vec3f(0, 0, 0)), specular_coef(1.f) {}
};

struct Object
{
    Material material;

    virtual bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const = 0;
    virtual Vec3f normal(const Vec3f& p) const = 0;
};
typedef std::vector<Object*> Objects;



struct Texture {

    int width;
    int height;
    TGAImage image;

    Texture(const char* filename) {
        
        image.read_tga_file(filename);
        this->width = image.get_width();
        this->height = image.get_height();
    }
    
    Vec3f getpixelColor(int x, int y) {
        TGAColor c = image.get(x, y);
        float r = (float)c.r / 255.f;
        float g = (float)c.g / 255.f;
        float b = (float)c.b / 255.f;
        return Vec3f(r, g, b);
    }


    
};

struct Cube : Object {

    Vec3f vmin;
    Vec3f vmax;

    Cube(const Vec3f& vmin1, const Vec3f vmax1, Material m) {

        vmax = vmax1;
        vmin = vmin1;
        Object::material = m;

    }


    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const{

        float txmin = (vmin[0] - p[0]) / d[0];
        float txmax = (vmax[0] - p[0]) / d[0];

        float tymin = ((vmin[1] - p[1]) / d[1]);
        float tymax = ((vmax[1] - p[1]) / d[1]);

        float tzmin = ((vmin[2] - p[2]) / d[2]);
        float tzmax = ((vmax[2] - p[2]) / d[2]);

        float txENT = min(txmin, txmax);
        float txEXT = max(txmin, txmax);
        float tyENT = min(tymin, tymax);
        float tyEXT = max(tymin, tymax);
        float tzENT = min(tzmin, tzmax);
        float tzEXT = max(tzmin, tzmax);

        if (txENT > tyEXT || txEXT < tyENT) {
            //cout << "1" << endl;
            return false;
        }
        else if (txENT > tzEXT || txEXT < tzENT) {
            //cout << "2" << endl;
            return false;
        }
        else {
            float mini = max(txENT, tyENT);
            float maxi = min(txEXT, tyEXT);
            t = max(mini, tzENT);
            if (t < 0) {
                t = min(maxi, tzEXT);
            }
            if (t < 0) {
                return false;
            }
            else {
                //cout << "3" << endl;
                return true;
            }
        }

    }

    Vec3f normal(const Vec3f& p) const {
        return (p - vmin).normalize();
    }

};

struct Sphere : Object
{
    Vec3f c; // centar
    float r; // radius

    Sphere(const Vec3f& c, const float& r, const Material& mat) : c(c), r(r)
    {
        Object::material = mat;
    }

    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const
    {
        Vec3f v = c - p; // vektor izmedju izvora zrake i centra

        if (d * v < 0) // skalarni produkt
        {
            // sfera se nalazi iza zrake
            return false;
        }
        else
        {
            // izracunaj projekciju
            Vec3f pc = p + d * ((d * v) / d.norm());
            if ((c - pc) * (c - pc) > r * r)
            {
                // nema sjeciste
                return false;
            }
            else
            {
                float dist = sqrt(r * r - (c - pc) * (c - pc));

                if (v * v > r * r) // izvor izvan sfere
                {
                    t = (pc - p).norm() - dist;
                }
                else // izvor unutar sfere
                {
                    t = (pc - p).norm() + dist;
                }

                return true;
            }
        }
    }

    Vec3f normal(const Vec3f& p) const
    {
        return (p - c).normalize();
    }
};

struct Cylinder : Object {
    
    Vec3f center;
    float radius;
    float height;

    Cylinder(Vec3f c, float r, float h, Material m) {
        center = c;
        radius = r;
        height = h;
        Object::material = m;
    }

    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const {
        
        Vec3f v = p - center;
            
        float a = (d.x * d.x) + (d.z * d.z);
        float b = 2 * (d.x * (p.x - center.x) + d.z * (p.z - center.z));
        float c = (p.x - center.x) * (p.x - center.x) + (p.z - center.z) * (p.z - center.z) - (radius * radius);

        float delta = b * b - 4 * (a * c);

        if (v * d < 0) {
            //cout << "1" << endl;
            return false;
        }
        else {

            if (delta < 0.0) {
                //cout << "2" << endl;
                return false;
            }

            float t1 = min((-b - sqrt(delta)) / (2 * a), (-b + sqrt(delta)) / (2 * a));
            float t2 = max((-b - sqrt(delta)) / (2 * a), (-b + sqrt(delta)) / (2 * a));


            float nek = p[1] + t1 * d[1];
            float nek2 = p[1] + t2 * d[1];


            if (nek >= center[1] && nek <= center[1] + height) {
                t = t1;
                return true;
            }
            else if (nek2 >= center[1] && nek2 <= center[1] + height) {
                t = t2;
                return true;
            }
            else {
                //cout << "3" << endl;
                return false;
            }
        }

    }

    Vec3f normal(const Vec3f& p) const {
        Vec3f n = (p - center).normalize();
        n.y = 0;
        //n = -n;
        return n;
    }


};

struct triangle : Object {

    Vec3f a, b, c;
    Vec3f normala = Vec3f(0, 0, 0);

    triangle(const Vec3f a, const Vec3f b, const Vec3f c, Material m) {
        this->a = a;
        this->b = b;
        this->c = c;
        Object::material = m;
    }

    triangle(const Vec3f a, const Vec3f b, const Vec3f c, const Vec3f normala, Material m) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->normala = normala;
        Object::material = m;
    }

    bool ray_intersect(const Vec3f& p, const Vec3f& d, float& t) const{
        /*float A = determinant(Vec3f(a.x - b.x, a.x - c.x, d.x), Vec3f(a.y - b.y, a.y - c.y, d.y), Vec3f(a.z - b.z, a.z - c.z, d.z));
        float beta = determinant(Vec3f(a.x - p.x, a.x - c.x, d.x), Vec3f(a.y - p.y, a.y - c.y, d.y), Vec3f(a.z - p.z, a.z - c.z, d.z)) / A;
        float gama = determinant(Vec3f(a.x - b.x, a.x - p.x, d.x), Vec3f(a.y - b.y, a.y - p.y, d.y), Vec3f(a.z - b.z, a.z - p.z, d.z)) / A;
        float t1 = determinant(Vec3f(a.x - b.x, a.x - c.x, a.x - p.x), Vec3f(a.y - b.y, a.y - c.y, a.y - p.y), Vec3f(a.z - b.z, a.z - c.z, a.z - p.z)) / A;
        float alpha = 1 - beta - gama;

        t = t1;

        if (t < 0) {
            return false;
        }

        if (alpha <= 1 && alpha >= 0 && beta <= 1 && beta >= 0 && gama <= 1, gama >= 0) {
            return true;
        }
        else {
            return false;
        }*/

        const float EPSILON = 0.0000001;
        Vec3f vertex0 = a;
        Vec3f vertex1 = b;
        Vec3f vertex2 = c;
        Vec3f edge1, edge2, h, s, q;
        float a, f, u, v;
        edge1 = vertex1 - vertex0;
        edge2 = vertex2 - vertex0;
        h = cross(d, edge2);
        a = edge1 * h;
        if (a > -EPSILON && a < EPSILON)
            return false;    // This ray is parallel to this triangle.
        f = 1.0 / a;
        s = p - vertex0;
        u = f * (s * h);
        if (u < 0.0 || u > 1.0)
            return false;
        q = cross(s, edge1);
        v = f * (d * q);
        if (v < 0.0 || u + v > 1.0)
            return false;
        // At this stage we can compute t to find out where the intersection point is on the line.
        t = f * (edge2 * q);
        if (t > EPSILON) // ray intersection
        {
            return true;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return false;
    }

    Vec3f normal(const Vec3f &p) const{
        if (normala.x != 0 || normala.y != 0 || normala.z != 0) {
            return normala;
        }
        else {
            return cross(b - a, c - a);
        }
    }

};

struct trianglemesh{
    vector<vector<float>> verteksi;
    vector<vector<map<int, int>>> facevi;
    vector<vector<float>> normale;
    string filename;
    vector<triangle> trokuti;
    Material m;

    trianglemesh(string filename, Material m) {
        this->filename = filename;
        this->m = m;
    }

    void loadMODEL() {
        OBJparser(filename, verteksi, facevi, normale);

        vector<Vec3f> tocke;
        for (auto face : facevi) {
            int thenormal = 0;
            for (auto koord : face) {
                Vec3f tocka;
                for (auto ver : koord) {

                    if (thenormal == 0) {
                        thenormal = ver.second;
                    }
                    tocka.x = verteksi[ver.first - 1][0];
                    tocka.y = verteksi[ver.first - 1][1];
                    tocka.z = verteksi[ver.first - 1][2];
                    tocke.push_back(tocka);
                }
            }
            
            thenormal--;
            Vec3f normala(normale[thenormal][0], normale[thenormal][1], normale[thenormal][2]);
            triangle t(tocke[0], tocke[1], tocke[2], normala.normalize() /*Vec3f(0, 0, 0)*/, m);
            trokuti.push_back(t);
            tocke.clear();
        }
    }
};

Vec3f view_transform(int nx, int ny, float n, float f, float r, float l, float t, float b, Vec3f u, Vec3f v, Vec3f w, Vec3f e, Vec3f tocka) {
    Matrix Morth(4, 4);
    Matrix Mcam(4, 4);
    Matrix Mvp(4, 4);
    Matrix P(4, 4);
   

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Morth[i][j] = Mcam[i][j] = Mvp[i][j] = P[i][j] = 0;
            if (i == j) {
                Morth[i][j] = Mcam[i][j] = Mvp[i][j] = P[i][j] = 1;
            }
        }
    }
    
    Mvp[0][0] = (float)nx / 2;
    Mvp[1][1] = (float)ny / 2;
    Mvp[0][3] = ((float)nx - 1) / 2;
    Mvp[1][3] = ((float)ny - 1) / 2;
    
    Morth[0][0] = 2 / (r - l);
    Morth[1][1] = 2 / (t - b);
    Morth[2][2] = 2 / (n - f);
    Morth[0][3] = -((r + l) / (r - l));
    Morth[1][3] = -((t + b) / (t - b));
    Morth[2][3] = -((n + f) / (n - f));

    
    P[0][0] = P[1][1] = n;
    P[2][2] = n + f;
    P[2][3] = -n * f;
    P[3][2] = 1;
    P[3][3] = 0;
    
    Matrix tockaM(4, 1);
    tockaM[0][0] = tocka.x;
    tockaM[1][0] = tocka.y;
    tockaM[2][0] = tocka.z;
    tockaM[3][0] = 1;

    
    
    float determinant = u.x * (v.y * w.z - w.y * v.z) + u.y * (v.x * w.z - w.x * v.z) + u.z * (v.x * w.y - w.x * v.y);
    
    if (determinant == 0) {
        determinant = 1;
    }

    Matrix adj(4, 4);
    Vec3f pomocni;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            
            if (i == 0) {
                pomocni = u;
                adj[i][j] = pomocni[j];
            }
            else if (i == 1) {
                pomocni = v;
                adj[i][j] = pomocni[j];
            }
            else if (i == 2){
                pomocni = w;
                adj[i][j] = pomocni[j];
            }
            else {
                pomocni = e;
                adj[i][j] = pomocni[j];
            }
            
        }
    }
    adj[0][3] = 0;
    adj[1][3] = 0;
    adj[2][3] = 0;
    adj[3][3] = 1;
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Mcam[i][j] = (1 / abs(determinant)) * adj[i][j];
        }
    }

    /*cout << "Mcam" << endl << endl;;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << Mcam[i][j] << " ";
        }
        cout << endl;
    }*/
    
    Matrix M1 = Mcam * tockaM;

    /*cout << "M1" << endl << endl;;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            cout << M1[i][j] << " ";
        }
        cout << endl;
    }*/

    Matrix M2 = P * M1;

    /*cout << "M2" << endl << endl;;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            cout << M2[i][j] << " ";
        }
        cout << endl;
    }*/

    Matrix M3 = Morth * M2;

    /*cout << "M3" << endl << endl;;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            cout << M3[i][j] << " ";
        }
        cout << endl;
    }*/

    Matrix M4 = Mvp * M3;

    /*cout << "M4" << endl << endl;;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            cout << M4[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Mvp" << endl << endl;;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << Mvp[i][j] << " ";
        }
        cout << endl;
    }*/

    for (int i = 0; i < 3; i++)
    {
        M4[i][0] = M4[i][0] / M4[3][0];
    }

    //cout << endl;
    return Vec3f(M4[0][0], M4[1][0], M4[2][0]).normalize();

}


// funkcija koja koristimo za crtanje scene
bool scene_intersect(const Vec3f& orig, const Vec3f& dir, const Objects& objs, Vec3f& hit_point, Material& hit_material, Vec3f& hit_normal)
{
    float dist = std::numeric_limits<float>::max();
    float obj_dist = std::numeric_limits<float>::max();
    for (auto obj : objs)
    {
        if (obj->ray_intersect(orig, dir, obj_dist) && obj_dist < dist)
        {
            dist = obj_dist;                     // udaljenost do sfere
            hit_point = dir * obj_dist;          // tocka sjecista
            hit_material = obj->material;        // materijal sfere
            hit_normal = obj->normal(hit_point); // normala na sjeciste
        }
    }
    return dist < 1000;
}

Vec3f reflex(Vec3f d, Vec3f n) {
    return d - ((n *(d * n)) * 2);
}

Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, const Objects& objs, const Lights& lights, int dubina, int i, int j, Texture& t)
{
    Vec3f hit_point;
    Vec3f hit_normal;
    Material hit_material;
    if (!scene_intersect(orig, dir, objs, hit_point, hit_material, hit_normal))
    {
        return t.getpixelColor(i, j); // vrati boju pozadine
    }
    else
    {
        float diffuse_color_intensity = 0;
        float specular_light_intensity = 0;


        for (auto light : lights)
        {
            Vec3f light_direction = (light.pos - hit_point).normalize();

            Vec3f shadow_origin;
            Vec3f shadow_hit_normal;
            Vec3f shadow_hitpoint;
            Material shadow_material;

            if (hit_normal * light_direction < 0) {
                hit_normal = -hit_normal;
            }

            if (light_direction * hit_normal < 0) {
                shadow_origin = hit_point - hit_normal * 0.001;
            }
            else {
                shadow_origin = hit_point + hit_normal * 0.001;
            }

            float light_dist = (light.pos - hit_point).norm();
            if (scene_intersect(shadow_origin, light_direction, objs, shadow_hitpoint, shadow_material, shadow_hit_normal) && (shadow_hitpoint - shadow_origin).norm() < light_dist) {
                continue;
            }

            // lambertov model
            diffuse_color_intensity += light.intenstity * max(0.f, light_direction * hit_normal);
            // bling phong model
            Vec3f v = (orig - hit_point).normalize();
            Vec3f h = (v + light_direction).normalize();

            specular_light_intensity += light.intenstity * powf(max(0.f, hit_normal * h), hit_material.specular_coef);
        }

        Vec3f R = reflex(dir, hit_normal);

        Vec3f c = hit_material.diffuse_color * hit_material.albedo[0] * diffuse_color_intensity + hit_material.diffuse_color * hit_material.albedo[1] * specular_light_intensity;

        if (dubina != 1) {
            return c + cast_ray(orig + hit_point * 0.00001, R, objs, lights, dubina - 1, i, j, t) * 0.2;
        }
        else if (dubina == 1) {
            return c;
        }

    }
}

// funkcija za crtanje
void render(const Objects& objects, const Lights& lights, Texture t)
{
    // velicina slike
    const int width = 1024;
    const int height = 768;
    const int fov = 3.1415926 / 2.0;

    // spremnik za sliku
    Image buffer(width * height);

    // nacrtaj u sliku
    for (size_t j = 0; j < height; ++j)
    {
        for (size_t i = 0; i < width; ++i)
        {
            // pošalji zraku u svaki piksel
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
            // definiraj smjer
            Vec3f dir = Vec3f(x, y, -1).normalize();
            

            float rad = 0.0174533;
            float a = cos(10 * rad);
            float b = sin(10 * rad);
            float c = -b;
            //pomak za 10 stupnjeva po y osi, vrijednosti directiona nisu dobri :(
            //dir = view_transform(1024, 786, 0.01, 1000, -50, 50, 50, -50, Vec3f(a, b, 0), Vec3f(0, 1, 0), Vec3f(c, a, 1), Vec3f(0, 0, 0), dir);
            //cout << dir << endl;

            buffer[i + j * width] = cast_ray(Vec3f(0, 0, 2), dir, objects, lights, 3, i, j, t);
        }
    }

    // spremanje slike u vanjsku datoteku
    std::ofstream ofs;

    //za primjenu transformacije treba upisati da rendera u datoteku transform ppm
    ofs.open("./sfere.ppm", ofstream::binary);
    // oblikuj po ppm formatu
    ofs << "P6\n"
        << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height * width; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            ofs << (unsigned char)(255.f * std::max(0.f, std::min(1.f, buffer[i][j])));
        }
    }
    // zatvori datoteku
    ofs.close();
}

int main()
{
    Material red(Vec2f(0.6, 0.4), Vec3f(1, 0, 0), 60);
    Material green(Vec2f(0.5, 0.5), Vec3f(0, 0.5, 0), 60);
    Material blue(Vec2f(0.7, 0.3), Vec3f(0, 0, 1), 10);
    Material grey(Vec2f(0.8, 0.2), Vec3f(0.5, 0.5, 0.5), 10);
    Material purple(Vec2f(0.4, 0.6), Vec3f(0.8, 0, 1), 30);

    // definiraj objekte u sceni
    Sphere s1(Vec3f(-3, 0, -16), 2, red);
    Sphere s2(Vec3f(-1.0, -1.5, -12), 2, purple);
    Sphere s3(Vec3f(1.5, -0.5, -18), 3, blue);
    Cube s4(Vec3f(2, 2, -20), Vec3f(5, 5, -25), blue);
    Cylinder cl1(Vec3f(15, 5, 30), 2, 8, purple);
    triangle t1(Vec3f(-1, -1.5, -10), Vec3f(-1.5, -2, -10), Vec3f(-2, -2, -10), grey);
    
    

    cout << endl;
   

    trianglemesh tm("gem.obj", purple);
    tm.loadMODEL();


    // definiranje svjetla
    Light l1 = Light(Vec3f(-20, 20, 20), 1.5);
    Light l2 = Light(Vec3f(20, 30, 20), 1.8);

    Lights lights = { l1, l2 };
    Objects objs = { &s1, &cl1, &t1, &s3 };
    for (int i = 0; i < tm.trokuti.size(); i++) {
        
        objs.push_back(&tm.trokuti[i]);

    }

    Texture t("mountains.tga");

    float rad = 0.0174533;
    float a = cos(10 * rad);
    float b = sin(10 * rad);
    float c = -b;
    Vec3f newform = view_transform(1024, 786, 0.01, 1000, -50, 50, 50, -50, Vec3f(a, b, 0), Vec3f(0, 1, 0), Vec3f(c, a, 1), Vec3f(0, 0, 0), Vec3f(0.5, 0.5, -1));
    cout << newform[0] << " " << newform[1] << " " << newform[2] << endl;
    render(objs, lights, t);


    cout << "Rendering complete" << endl;
    return 0;
}