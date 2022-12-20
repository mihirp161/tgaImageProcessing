#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <random>
//#include <algorithm>
//#include "leaker.h"

using namespace std;

class Image {
private:
    vector<string> stored_images{ };
public:
    // header interface
    struct Header {
        friend class Image;


        Header() : idLength(), colorMapType(),
            dataTypeCode(), colorMapOrigin(), colorMapLength(),
            colorMapDepth(), xOrigin(), yOrigin(), imageWidth(),
            imageHeight(), bitsPerPixel(), imageDescriptor() { }

        Header(
            char _idLength, char _colorMapType, char _dataTypeCode, short _colorMapOrigin, short _colorMapLength,
            char _colorMapDepth, short _xOrigin, short _yOrigin, short _imageWidth, short _imageHeight,
            char _bitsPerPixel, char _imageDescriptor) : idLength(_idLength), colorMapType(_colorMapType),
            dataTypeCode(_dataTypeCode), colorMapOrigin(_colorMapOrigin), colorMapLength(_colorMapLength),
            colorMapDepth(_colorMapDepth), xOrigin(_xOrigin), yOrigin(_yOrigin), imageWidth(_imageWidth),
            imageHeight(_imageHeight), bitsPerPixel(_bitsPerPixel), imageDescriptor(_imageDescriptor) {

        }

        char idLength;
        char colorMapType;
        char dataTypeCode;
        short colorMapOrigin;
        short colorMapLength;
        char colorMapDepth;
        short xOrigin;
        short yOrigin;
        short imageWidth;
        short imageHeight;
        char bitsPerPixel;
        char imageDescriptor;


        virtual ~Header() { }

    };

    // pixel interface
    struct Pixel {
        friend class Image;

        Pixel() : blue(), green(), red() { }

        Pixel(unsigned char _blue, unsigned char _green, unsigned char _red) : blue(_blue), green(_green), red(_red) {

        }

        unsigned char blue;
        unsigned char green;
        unsigned char red;

        virtual ~Pixel() { }

    };

    /*============ Image stuff ===============*/

    void prnt_stord_imgs_info() {

        for (string& s : stored_images) {
            cout << s << endl;
        }

    }

    // reads a tga file. Takes the file path only.
    void read_Image(string& file_path);

    // write a tga file. Takes the file path and which element to print.
    void write_Image(string& file_path, int& index);

    // generate a new image and push it into the header and pixel vector
    void gener_new_img(int& borrow_hdr_at_i, string& img_name, bool to_randomize = true,
        unsigned char _blue = 0, unsigned char _green = 0,
        unsigned char _red = 0);

    // extract the pixels from an given image
    void extract_pxls(int& borrow_info_at_i, string& img_name, string which_pxls = "red");

    // mix and match the pixels from different images
    void mix_pxls_n_generate(int& borrow_hdr_at_i, int& borrow_rPxls_at_i, int& borrow_gPxls_at_i, int& borrow_bPxls_at_i, string& img_name);

    //iterates through header values and compare them
    bool compare_header(int& hdr_at_i, int& hdr_at_j);

    // iterates through pixel values and compare them
    bool compare_pixels(int& pxl_at_i, int& pxl_at_j);

    // compares the headers and pixels
    bool compare_Image(int& a, int& b);

    // multiplies the pixels from two vectors and creates new image
    void multiplication_blend(int& top_img_ind, int& bott_img_ind, string& img_name);

    // subtract the pixels from two vectors and creates new image
    void subtraction_blend(int& top_img_ind, int& bott_img_ind, string& img_name);

    // screen the pixels from two vectors and creates new image
    void screen_blend(int& top_img_ind, int& bott_img_ind, string& img_name);

    // overlay the pixels from two vectors and creates new image
    void overlay_blend(int& top_img_ind, int& bott_img_ind, string& img_name);

    // flip an image 180 degrees
    void rotate_img180(int& img_ind, string& img_name);

    // creates quadrants of images
    void create_img_quad(int& borrow_hdr_at_i, int& top_left, int& top_right, int& bott_left, int& bott_right, string& img_name);

    // return two largest heights from a given vector
    vector<short> return_two_lrg_vals(vector<short>& short_vector_vals);


    /*============== Header stuff ==============*/
    vector<Header> header_vector{ };

    static int img_hdr_key;  // counters to keep track where which images are stored

    /*============== Pixel stuff ==============*/

    map<int, vector<Pixel>> pixel_map{ };

    // randomizes the pixels values and changes the rgb values in all the stored images
    void randomize_pxls_in_all();

    // add to the pixel values
    void add_to_blue_pxls(int& img_ind, unsigned char increase_by);
    void add_to_green_pxls(int& img_ind, unsigned char increase_by);
    void add_to_red_pxls(int& img_ind, unsigned char increase_by);

    // scale the pixel values
    void scale_blue_pxls(int& img_ind, unsigned char scale_by);
    void scale_green_pxls(int& img_ind, unsigned char scale_by);
    void scale_red_pxls(int& img_ind, unsigned char scale_by);

    // give size of pixel channel stored inside a pixel vector
    long int pixel_channel_size(int& index);

    // noarmalize the values
    unsigned char normalize(unsigned char& a, unsigned char& b);

    // clamp the values
    unsigned char clamp(unsigned char& a, unsigned char& b, string which_clamp = "subtracting");

    // factor based normalization the values
    unsigned char factored_normalize(unsigned char& a, unsigned char& b, bool to_factor, float fctr = 1.0f);

};

