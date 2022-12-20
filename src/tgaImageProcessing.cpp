#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "Image.h"


using namespace std;
using namespace std::chrono;

static int pass_counter = 0;

string return_pass_fail(bool result) {

    if (result == 1) {
        pass_counter++;
        return (string("Passed!"));
    }
    else {
        return (string("Failed!"));
    }
}

int main() {

    Image image;

    //file names
    string in_path;
    string out_path;
    string temp_path;

    //indices
    int img_a;
    int img_b;

    //******************************** Multiplication blend *************************

    in_path = "./input/layer1.tga"; //0 index
    image.read_Image(in_path);

    in_path = "./input/pattern1.tga"; //1 index
    image.read_Image(in_path);

    out_path = "./output/part1.tga";

    img_a = 0;
    img_b = 1;

    image.multiplication_blend(img_a, img_b, out_path); // 2 index

    in_path = "./examples/EXAMPLE_part1.tga"; //3 index
    image.read_Image(in_path);

    img_a = 2;
    img_b = 3;

    cout << "multiplication_blend...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;
    image.write_Image(out_path, img_a);


    // ********************** Subtraction blend *******************************

    in_path = "./input/layer2.tga"; //4
    image.read_Image(in_path);

    in_path = "./input/car.tga"; //5
    image.read_Image(in_path);

    out_path = "./output/part2.tga";

    img_a = 4;
    img_b = 5;

    image.subtraction_blend(img_a, img_b, out_path); //6

    in_path = "./examples/EXAMPLE_part2.tga"; //7
    image.read_Image(in_path);

    img_a = 6;
    img_b = 7;

    cout << "subtraction_blend...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    image.write_Image(out_path, img_a);

    // ********************** Screen blend *******************************

    in_path = "./input/layer1.tga"; //8
    image.read_Image(in_path);

    temp_path = in_path;

    in_path = "./input/pattern2.tga"; //9
    image.read_Image(in_path);

    temp_path += " * " + in_path + " <----- TEMP";

    img_a = 8;
    img_b = 9;

    image.multiplication_blend(img_a, img_b, temp_path); //10

    in_path = "./input/circles.tga"; // 11
    image.read_Image(in_path);

    img_a = 11;
    img_b = 10;

    out_path = "./output/part3.tga";

    image.screen_blend(img_a, img_b, out_path); //12

    in_path = "./examples/EXAMPLE_part3.tga"; //13
    image.read_Image(in_path);

    img_a = 12;
    img_b = 13;

    cout << "screen_blend...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    img_a = 12;
    image.write_Image(out_path, img_a);

    // ********************** Subtraction blend *******************************

    in_path = "./input/layer2.tga"; //14
    image.read_Image(in_path);

    temp_path = in_path;

    in_path = "./input/circles.tga"; //15
    image.read_Image(in_path);

    temp_path += " * " + in_path + " <----- TEMP";

    img_a = 14;
    img_b = 15;

    image.multiplication_blend(img_a, img_b, temp_path); //16

    in_path = "./input/pattern2.tga"; //17
    image.read_Image(in_path);

    img_a = 17;
    img_b = 16;

    out_path = "./output/part4.tga";

    image.subtraction_blend(img_a, img_b, out_path); //18

    in_path = "./examples/EXAMPLE_part4.tga"; //19
    image.read_Image(in_path);

    img_a = 18;
    img_b = 19;

    cout << "subtraction_blend...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    img_a = 18;
    image.write_Image(out_path, img_a);

    // ********************** Overlay blend *******************************

    in_path = "./input/layer1.tga"; //20
    image.read_Image(in_path);

    in_path = "./input/pattern1.tga"; //21
    image.read_Image(in_path);

    out_path = "./output/part5.tga";

    img_a = 20;
    img_b = 21;

    image.overlay_blend(img_a, img_b, out_path); //22

    in_path = "./examples/EXAMPLE_part5.tga"; //23
    image.read_Image(in_path);

    img_a = 22;
    img_b = 23;

    cout << "overlay_blend...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    image.write_Image(out_path, img_a);

    // ********************** Add to green pixels *******************************

    in_path = "./input/car.tga"; //24
    image.read_Image(in_path);

    out_path = "./output/part6.tga";

    img_a = 24;
    img_b = 999;

    image.add_to_green_pxls(img_a, 200);

    in_path = "./examples/EXAMPLE_part6.tga"; //25
    image.read_Image(in_path);

    img_a = 24;
    img_b = 25;

    cout << "add_to_green_pxls...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    image.write_Image(out_path, img_a);

    // ********************** Scale some pixels *******************************

    in_path = "./input/car.tga"; //26
    image.read_Image(in_path);

    out_path = "./output/part7.tga";

    img_a = 26;
    img_b = 999;

    image.scale_red_pxls(img_a, 4);
    image.scale_blue_pxls(img_a, 0);

    in_path = "./examples/EXAMPLE_part7.tga"; //27
    image.read_Image(in_path);

    img_a = 26;
    img_b = 27;

    cout << "scale_red_pxls & scale_blue_pxls...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    image.write_Image(out_path, img_a);

    // ********************** Extract pixels *******************************

    in_path = "./input/car.tga"; //28
    image.read_Image(in_path);

    out_path = "./output/part8_r.tga";

    img_a = 28;
    img_b = 999;

    image.extract_pxls(img_a, out_path, "red"); //29

    in_path = "./examples/EXAMPLE_part8_r.tga"; // 30
    image.read_Image(in_path);

    img_a = 29;
    img_b = 30;

    cout << "extract_pxls-red...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    image.write_Image(out_path, img_a);

    // ********************** Extract pixels *******************************

    in_path = "./input/car.tga"; //31
    image.read_Image(in_path);

    out_path = "./output/part8_g.tga";

    img_a = 31;
    img_b = 999;

    image.extract_pxls(img_a, out_path, "green"); //32

    in_path = "./examples/EXAMPLE_part8_g.tga"; //33
    image.read_Image(in_path);

    img_a = 32;
    img_b = 33;

    cout << "extract_pxls-green...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    image.write_Image(out_path, img_a);

    // ********************** Extract pixels *******************************

    in_path = "./input/car.tga"; //34
    image.read_Image(in_path);

    out_path = "./output/part8_b.tga";

    img_a = 34;
    img_b = 999;

    image.extract_pxls(img_a, out_path, "blue"); //35

    in_path = "./examples/EXAMPLE_part8_b.tga"; //36
    image.read_Image(in_path);

    img_a = 35;
    img_b = 36;

    cout << "extract_pxls-blue...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    image.write_Image(out_path, img_a);

    // ********************** Mix pixles then generate an image *******************************

    in_path = "./input/layer_red.tga"; //37
    image.read_Image(in_path);

    in_path = "./input/layer_green.tga"; //38
    image.read_Image(in_path);

    in_path = "./input/layer_blue.tga"; //39
    image.read_Image(in_path);

    out_path = "./output/part9.tga";

    img_a = 999;
    img_b = 999;

    int ir = 37;
    int ig = 38;
    int ib = 39;

    image.mix_pxls_n_generate(ir, ir, ig, ib, out_path); //40

    in_path = "./examples/EXAMPLE_part9.tga"; //41
    image.read_Image(in_path);

    img_a = 40;
    img_b = 41;

    cout << "mix_pxls_n_generate...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    image.write_Image(out_path, img_a);

    // ********************** Create image quadrant *******************************
    in_path = "./input/car.tga"; //42
    image.read_Image(in_path);

    in_path = "./output/part6.tga"; //43
    image.read_Image(in_path);

    in_path = "./output/part8_g.tga"; //44
    image.read_Image(in_path);

    in_path = "./output/part7.tga"; //45
    image.read_Image(in_path);

    out_path = "./output/part10.tga";

    img_a = 999;
    img_b = 999;

    int tl = 42;
    int tr = 43;
    int bl = 44;
    int br = 45;

    image.create_img_quad(tl, tl, tr, bl, br, out_path); //46

    in_path = "./examples/EXAMPLE_part11.tga"; //47
    image.read_Image(in_path);

    img_a = 46;
    img_b = 47;

    cout << "create_img_quad...... " << return_pass_fail(image.compare_Image(img_a, img_b)) << endl;

    image.write_Image(out_path, img_a);

    //cout << "Passed: " << pass_counter << " / 12" << endl;
    
    //image.prnt_stord_imgs_info(); // gives stored images information

    return 0;
}

