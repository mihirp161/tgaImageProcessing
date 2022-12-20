#include "Image.h"

/*===================================== Image stuff ======================================================*/

// reads a tga file. Takes the file path only.
void Image::read_Image(string& file_path) {

    // open in binary mode
    ifstream img_file(file_path, ios::binary | ios::in);

    //check
    if (!img_file.is_open()) {
        cout << "Reading binary file " << file_path << " failed." << endl;
        EXIT_FAILURE;
    }

    // if the file is open store the header and pixel values
    if (img_file.is_open()) {

        Header hdr;

        img_file.read(reinterpret_cast<char*> (&hdr.idLength), sizeof(hdr.idLength));
        img_file.read(reinterpret_cast<char*> (&hdr.colorMapType), sizeof(hdr.colorMapType));
        img_file.read(reinterpret_cast<char*> (&hdr.dataTypeCode), sizeof(hdr.dataTypeCode));
        img_file.read(reinterpret_cast<char*> (&hdr.colorMapOrigin), sizeof(hdr.colorMapOrigin));
        img_file.read(reinterpret_cast<char*> (&hdr.colorMapLength), sizeof(hdr.colorMapLength));
        img_file.read(reinterpret_cast<char*> (&hdr.colorMapDepth), sizeof(hdr.colorMapDepth));
        img_file.read(reinterpret_cast<char*> (&hdr.xOrigin), sizeof(hdr.xOrigin));
        img_file.read(reinterpret_cast<char*> (&hdr.yOrigin), sizeof(hdr.yOrigin));
        img_file.read(reinterpret_cast<char*> (&hdr.imageWidth), sizeof(hdr.imageWidth));
        img_file.read(reinterpret_cast<char*> (&hdr.imageHeight), sizeof(hdr.imageHeight));
        img_file.read(reinterpret_cast<char*> (&hdr.bitsPerPixel), sizeof(hdr.bitsPerPixel));
        img_file.read(reinterpret_cast<char*> (&hdr.imageDescriptor), sizeof(hdr.imageDescriptor));

        header_vector.push_back(hdr);
        Pixel pxl;

        vector<Pixel> temp{};

        for (int i = 0; i < static_cast<int>(hdr.imageWidth * hdr.imageHeight); i++) {

            img_file.read(reinterpret_cast<char*> (&pxl.blue), sizeof(pxl.blue));
            img_file.read(reinterpret_cast<char*> (&pxl.green), sizeof(pxl.green));
            img_file.read(reinterpret_cast<char*> (&pxl.red), sizeof(pxl.red));

            temp.push_back(pxl);
        }

        // make a map so a correct pixel values can be extracted belonging to a vector
        pixel_map.insert(make_pair(img_hdr_key, temp));

        img_file.close();

        stored_images.push_back(string(to_string(img_hdr_key) + ": " + file_path));

        img_hdr_key++;

    }
}

// write a tga file. Takes the file path and which element to print.
void Image::write_Image(string& file_path, int& index) {

    // open in binary mode
    ofstream img_file(file_path, ios::binary | ios::out);

    //check
    if (!img_file.is_open()) {
        cout << "Writing binary file " << file_path << " failed." << endl;
        EXIT_FAILURE;
    }
    // when file is open first write header then pixel values
    if (img_file.is_open()) {

        Header hdr = header_vector.at(index);

        img_file.write(reinterpret_cast<char*>(&hdr.idLength), sizeof(hdr.idLength));
        img_file.write(reinterpret_cast<char*>(&hdr.colorMapType), sizeof(hdr.colorMapType));
        img_file.write(reinterpret_cast<char*>(&hdr.dataTypeCode), sizeof(hdr.dataTypeCode));
        img_file.write(reinterpret_cast<char*>(&hdr.colorMapOrigin), sizeof(hdr.colorMapOrigin));
        img_file.write(reinterpret_cast<char*>(&hdr.colorMapLength), sizeof(hdr.colorMapLength));
        img_file.write(reinterpret_cast<char*>(&hdr.colorMapDepth), sizeof(hdr.colorMapDepth));
        img_file.write(reinterpret_cast<char*>(&hdr.xOrigin), sizeof(hdr.xOrigin));
        img_file.write(reinterpret_cast<char*>(&hdr.yOrigin), sizeof(hdr.yOrigin));
        img_file.write(reinterpret_cast<char*>(&hdr.imageWidth), sizeof(hdr.imageWidth));
        img_file.write(reinterpret_cast<char*>(&hdr.imageHeight), sizeof(hdr.imageHeight));
        img_file.write(reinterpret_cast<char*>(&hdr.bitsPerPixel), sizeof(hdr.bitsPerPixel));
        img_file.write(reinterpret_cast<char*>(&hdr.imageDescriptor), sizeof(hdr.imageDescriptor));

        // pixel belonging to a it's parent header 
        vector<Pixel> pxl = pixel_map.at(index);

        for (unsigned int i = 0; i < pxl.size(); i++) {


            img_file.write(reinterpret_cast<char*> (&pxl.at(i).blue), sizeof(pxl.at(i).blue));
            img_file.write(reinterpret_cast<char*> (&pxl.at(i).green), sizeof(pxl.at(i).green));
            img_file.write(reinterpret_cast<char*> (&pxl.at(i).red), sizeof(pxl.at(i).red));


        }

        img_file.close();
    }

}

// generate a new image and push it into the header and pixel vector
void Image::gener_new_img(int& borrow_hdr_at_i, string& img_name, bool to_randomize, unsigned char _blue,
    unsigned char _green, unsigned char _red) {

    if (borrow_hdr_at_i < 0 || borrow_hdr_at_i > static_cast<int>(header_vector.size()) || header_vector.size() == 0) {
        cout << "gener_new_img: Index of bounds or no image loaded." << endl;
        return;
    }
    // copy a pictures information
    Header hdr = header_vector.at(borrow_hdr_at_i);
    // push this new value in a vector
    header_vector.push_back(hdr);

    // if user wants to randomize vector values
    Pixel pxl;
    int myseed = 565;

    default_random_engine rng(myseed);
    uniform_int_distribution<int> rng_dist(0, 255);

    unsigned char rand_blue;
    unsigned char rand_green;
    unsigned char rand_red;

    if (to_randomize == true) {
        rand_blue = static_cast<unsigned char>(rng_dist(rng));
        rand_green = static_cast<unsigned char>(rng_dist(rng));
        rand_red = static_cast<unsigned char>(rng_dist(rng));
    }
    else {
        rand_blue = _blue;
        rand_green = _green;
        rand_red = _red;
    }
    // push all the pixels in the vector then store it in the map
    vector<Pixel> temp{};

    // run the loop image's height*width time
    for (int i = 0; i < static_cast<int>(hdr.imageWidth * hdr.imageHeight); i++) {

        pxl.blue = rand_blue;
        pxl.green = rand_green;
        pxl.red = rand_red;

        temp.push_back(pxl);
    }

    pixel_map.insert(make_pair(img_hdr_key, temp));
    stored_images.push_back(string(to_string(img_hdr_key) + ": " + img_name));
    img_hdr_key++;
}

// extract the pixels from an given image
void Image::extract_pxls(int& borrow_info_at_i, string& img_name, string which_pxls) {

    if (borrow_info_at_i < 0 || borrow_info_at_i >(int) header_vector.size() || header_vector.size() == 0) {
        cout << "extract_pxls: Index of bounds or no image loaded." << endl;
        return;
    }

    Header hdr = header_vector.at(borrow_info_at_i);

    header_vector.push_back(hdr);

    Pixel pxl;
    vector<Pixel> temp{};
    // load the pixels of the current image
    vector<Pixel> img_pxls = pixel_map.at(borrow_info_at_i);

    // which pixels to overwrite

    for (int i = 0; i < static_cast<int>(hdr.imageWidth * hdr.imageHeight); i++) {
        // change values
        if (which_pxls.compare("red") == 0) {
            pxl.red = img_pxls.at(i).red;
            pxl.blue = img_pxls.at(i).red;
            pxl.green = img_pxls.at(i).red;
        }
        else if (which_pxls.compare("green") == 0) {
            pxl.red = img_pxls.at(i).green;
            pxl.blue = img_pxls.at(i).green;
            pxl.green = img_pxls.at(i).green;
        }
        else if (which_pxls.compare("blue") == 0) {
            pxl.red = img_pxls.at(i).blue;
            pxl.blue = img_pxls.at(i).blue;
            pxl.green = img_pxls.at(i).blue;

        }

        temp.push_back(pxl);
    }
    // store in the pixel map as a new map
    pixel_map.insert(make_pair(img_hdr_key, temp));
    stored_images.push_back(string(to_string(img_hdr_key) + ": " + img_name));
    img_hdr_key++;
}


// mix and match the pixels from different images
void Image::mix_pxls_n_generate(int& borrow_hdr_at_i, int& borrow_rPxls_at_i, int& borrow_gPxls_at_i, int& borrow_bPxls_at_i, string& img_name) {

    if (header_vector.size() <= 2 || borrow_hdr_at_i > (int)header_vector.size() || borrow_hdr_at_i < 0) {
        cout << "mix_pxls_n_generate: Please load at least 3 images." << endl;
        return;
    }

    Header hdr = header_vector.at(borrow_hdr_at_i);

    header_vector.push_back(hdr);

    Pixel pxl;
    vector<Pixel> temp{};
    // get all the rgb values in a vector
    vector<Pixel> red_pxls = pixel_map.at(borrow_rPxls_at_i);
    vector<Pixel> green_pxls = pixel_map.at(borrow_gPxls_at_i);
    vector<Pixel> blue_pxls = pixel_map.at(borrow_bPxls_at_i);

    // store all the pixels
    for (int i = 0; i < static_cast<int>(hdr.imageWidth * hdr.imageHeight); i++) {

        pxl.red = red_pxls.at(i).red;
        pxl.blue = blue_pxls.at(i).blue;
        pxl.green = green_pxls.at(i).green;

        temp.push_back(pxl);
    }

    pixel_map.insert(make_pair(img_hdr_key, temp));
    stored_images.push_back(string(to_string(img_hdr_key) + ": " + img_name));
    img_hdr_key++;

}

//iterates through header values and compare them
bool Image::compare_header(int& hdr_at_i, int& hdr_at_j) {

    if (header_vector.size() <= 1) {
        cout << "compare_header: Vector size is 1. Need an image to compare." << endl;
        return false;
    }
    else if (header_vector.size() > 1) {

        if (header_vector.at(hdr_at_i).idLength != header_vector.at(hdr_at_j).idLength ||
            header_vector.at(hdr_at_i).colorMapType != header_vector.at(hdr_at_j).colorMapType ||
            header_vector.at(hdr_at_i).dataTypeCode != header_vector.at(hdr_at_j).dataTypeCode ||
            header_vector.at(hdr_at_i).colorMapOrigin != header_vector.at(hdr_at_j).colorMapOrigin ||
            header_vector.at(hdr_at_i).colorMapLength != header_vector.at(hdr_at_j).colorMapLength ||
            header_vector.at(hdr_at_i).colorMapDepth != header_vector.at(hdr_at_j).colorMapDepth ||
            header_vector.at(hdr_at_i).xOrigin != header_vector.at(hdr_at_j).xOrigin ||
            header_vector.at(hdr_at_i).yOrigin != header_vector.at(hdr_at_j).yOrigin ||
            header_vector.at(hdr_at_i).imageWidth != header_vector.at(hdr_at_j).imageWidth ||
            header_vector.at(hdr_at_i).imageHeight != header_vector.at(hdr_at_j).imageHeight ||
            header_vector.at(hdr_at_i).bitsPerPixel != header_vector.at(hdr_at_j).bitsPerPixel ||
            header_vector.at(hdr_at_i).imageDescriptor != header_vector.at(hdr_at_j).imageDescriptor) {
            return false;
        }

    }

    return true;
}

// iterates through pixel values and compare them
bool Image::compare_pixels(int& pxl_at_i, int& pxl_at_j) {

    if (pixel_map.size() <= 1) {
        cout << "compare_pixels: Vector size is 1. Need an image to compare." << endl;
        return false;
    }
    // if a valid header exists for a pixel
    else if (compare_header(pxl_at_i, pxl_at_j)) {

        // get the pixel lengths
        long int img_1_pxls = pixel_channel_size(pxl_at_i);
        long int img_2_pxls = pixel_channel_size(pxl_at_j);

        if (img_1_pxls != img_2_pxls) {
            cout << "compare_pixels: Pixel channels are not equal." << endl;
            return false;
        }
        else {
            // if the pixels channels are equal length then comare them based on rgb values
            vector<Pixel> img_1_pxls = pixel_map.at(pxl_at_i);
            vector<Pixel> img_2_pxls = pixel_map.at(pxl_at_j);

            for (unsigned int i = 0, j = 0; i < img_1_pxls.size() && j < img_2_pxls.size(); i++, j++) {

                if (img_1_pxls.at(i).blue != img_2_pxls.at(j).blue ||
                    img_1_pxls.at(i).green != img_2_pxls.at(j).green ||
                    img_1_pxls.at(i).red != img_2_pxls.at(j).red) {

                    return false;
                }
            }
        }

    }


    return true;
}

// compares the headers and pixels
bool Image::compare_Image(int& a, int& b) {
    // return true if both header and pixels are matching
    return (compare_header(a, b) && compare_pixels(a, b));
}

// multiplies the pixels from two vectors and creates new image
void Image::multiplication_blend(int& top_img_ind, int& bott_img_ind, string& img_name) {

    if (pixel_map.size() <= 0) {
        cout << "multiplication_blend: No images. Read at least two images." << endl;
        return;
    }
    // get the header from top image
    Header hdr = header_vector.at(top_img_ind);
    header_vector.push_back(hdr);

    Pixel pxl;
    vector<Pixel> temp{};
    // get the pixels inside both images
    vector<Pixel> top_img_pxls = pixel_map.at(top_img_ind);
    vector<Pixel> bott_img_pxls = pixel_map.at(bott_img_ind);

    //normalize the pixels and store them in vector
    for (int i = 0; i < static_cast<int>(hdr.imageWidth * hdr.imageHeight); i++) {

        pxl.blue = normalize(top_img_pxls.at(i).blue, bott_img_pxls.at(i).blue);

        pxl.green = normalize(top_img_pxls.at(i).green, bott_img_pxls.at(i).green);

        pxl.red = normalize(top_img_pxls.at(i).red, bott_img_pxls.at(i).red);

        temp.push_back(pxl);
    }
    // push the vector in a map
    pixel_map.insert(make_pair(img_hdr_key, temp));
    stored_images.push_back(string(to_string(img_hdr_key) + ": " + img_name));
    img_hdr_key++;
}

// subtract the pixels from two vectors and creates new image
void Image::subtraction_blend(int& top_img_ind, int& bott_img_ind, string& img_name) {

    if (pixel_map.size() <= 0) {
        cout << "subtraction_blend: No images. Read at least two images." << endl;
        return;
    }
    // get the header
    Header hdr = header_vector.at(top_img_ind);
    header_vector.push_back(hdr);

    // get the pixels
    Pixel pxl;
    vector<Pixel> temp{};
    // get the pixels
    vector<Pixel> top_img_pxls = pixel_map.at(top_img_ind);
    vector<Pixel> bott_img_pxls = pixel_map.at(bott_img_ind);

    // clamp the values to prevent over or underflow
    for (int i = 0; i < static_cast<int>(hdr.imageWidth * hdr.imageHeight); i++) {

        pxl.blue = clamp(top_img_pxls.at(i).blue, bott_img_pxls.at(i).blue);

        pxl.green = clamp(top_img_pxls.at(i).green, bott_img_pxls.at(i).green);

        pxl.red = clamp(top_img_pxls.at(i).red, bott_img_pxls.at(i).red);

        temp.push_back(pxl);
    }

    pixel_map.insert(make_pair(img_hdr_key, temp));

    stored_images.push_back(string(to_string(img_hdr_key) + ": " + img_name));

    img_hdr_key++;
}

// screen the pixels from two vectors and creates new image
void Image::screen_blend(int& top_img_ind, int& bott_img_ind, string& img_name) {

    if (pixel_map.size() <= 0) {
        cout << "screen_blend: No images. Read at least two images." << endl;
        return;
    }
    // get header from top image
    Header hdr = header_vector.at(top_img_ind);
    header_vector.push_back(hdr);

    Pixel pxl;
    vector<Pixel> temp{};

    //get pixels
    vector<Pixel> top_img_pxls = pixel_map.at(top_img_ind);
    vector<Pixel> bott_img_pxls = pixel_map.at(bott_img_ind);

    // normalize the pixel values with 1.0f factor and store them in the map
    for (int i = 0; i < static_cast<int>(hdr.imageWidth * hdr.imageHeight); i++) {

        pxl.blue = factored_normalize(top_img_pxls.at(i).blue, bott_img_pxls.at(i).blue, true, 1.0f);

        pxl.green = factored_normalize(top_img_pxls.at(i).green, bott_img_pxls.at(i).green, true, 1.0f);

        pxl.red = factored_normalize(top_img_pxls.at(i).red, bott_img_pxls.at(i).red, true, 1.0f);

        temp.push_back(pxl);
    }

    pixel_map.insert(make_pair(img_hdr_key, temp));

    stored_images.push_back(string(to_string(img_hdr_key) + ": " + img_name));

    img_hdr_key++;
}

// overlay the pixels from two vectors and creates new image
void Image::overlay_blend(int& top_img_ind, int& bott_img_ind, string& img_name) {

    if (pixel_map.size() <= 0) {
        cout << "overlay_blend: No images. Read at least two images." << endl;
        return;
    }

    Header hdr = header_vector.at(top_img_ind);

    header_vector.push_back(hdr);

    Pixel pxl;
    vector<Pixel> temp{};

    // invert the pixels for the overlay
    vector<Pixel> top_img_pxls = pixel_map.at(bott_img_ind);
    vector<Pixel> bott_img_pxls = pixel_map.at(top_img_ind);

    for (int i = 0; i < static_cast<int>(hdr.imageWidth * hdr.imageHeight); i++) {

        // check if current pixel is gray
        if (static_cast<float>(top_img_pxls.at(i).blue / 255.0f) <= 0.5f) {

            // if gray then don't multiply by 2.0f factor. Just normalize it
            pxl.blue = factored_normalize(top_img_pxls.at(i).blue, bott_img_pxls.at(i).blue, false);
        }
        else {
            // if gray then multiply by 2.0f factor. Then normalize it
            pxl.blue = factored_normalize(top_img_pxls.at(i).blue, bott_img_pxls.at(i).blue, true, 2.0f);

        }

        if (static_cast<float>(top_img_pxls.at(i).green / 255.0f) < 0.5f) {

            pxl.green = factored_normalize(top_img_pxls.at(i).green, bott_img_pxls.at(i).green, false);
        }
        else {
            pxl.green = factored_normalize(top_img_pxls.at(i).green, bott_img_pxls.at(i).green, true, 2.0f);
        }

        if (static_cast<float>(top_img_pxls.at(i).red / 255.0f) <= 0.5f) {

            pxl.red = factored_normalize(top_img_pxls.at(i).red, bott_img_pxls.at(i).red, false);
        }
        else {
            pxl.red = factored_normalize(top_img_pxls.at(i).red, bott_img_pxls.at(i).red, true, 2.0f);
        }

        temp.push_back(pxl);
    }

    // store the info in the map
    pixel_map.insert(make_pair(img_hdr_key, temp));

    stored_images.push_back(string(to_string(img_hdr_key) + ": " + img_name));

    img_hdr_key++;

}

// flip an image 180 degrees
void Image::rotate_img180(int& img_ind, string& img_name) {

    if (header_vector.size() <= 0) {
        cout << "rotate_img180: Please load at least 1 image." << endl;
        return;
    }

    // push the header values
    Header hdr = header_vector.at(img_ind);
    header_vector.push_back(hdr);

    Pixel pxl;
    // get the pixels
    vector<Pixel>& temp = pixel_map.at(img_ind);
    vector<Pixel> p_bin{};
    // flip the pixels vector (not supported in c++11??)
    //reverse(temp.begin(), temp.end());

    for (vector<Pixel>::reverse_iterator it = temp.rbegin(); it != temp.rend(); it++) {
        pxl.blue = it->blue;
        pxl.green = it->green;
        pxl.red = it->red;

        p_bin.push_back(pxl);
    }

    //pixel_map.insert(make_pair(img_hdr_key, temp));
    pixel_map.insert(make_pair(img_hdr_key, p_bin));
    stored_images.push_back(string(to_string(img_hdr_key) + ": " + img_name));
    img_hdr_key++;

}


// creates quadrants of images
void Image::create_img_quad(int& borrow_hdr_at_i, int& top_left, int& top_right, int& bott_left, int& bott_right, string& img_name) {

    if (header_vector.size() < 4) {
        cout << "create_img_quad: Please load at least 4 images." << endl;
        return;
    }

    // get all the heights and widths of vectors
    vector<short> all_width = { header_vector.at(top_left).imageWidth , header_vector.at(top_right).imageWidth,
                     header_vector.at(bott_left).imageWidth , header_vector.at(bott_right).imageWidth };

    vector<short> all_heights = { header_vector.at(top_left).imageHeight , header_vector.at(top_right).imageHeight,
                     header_vector.at(bott_left).imageHeight , header_vector.at(bott_right).imageHeight };

    // added this check later to see if all the pictures are going to be same size
    if (!equal(all_width.begin() + 1, all_width.end(), all_width.begin()) ||
        !equal(all_heights.begin() + 1, all_heights.end(), all_heights.begin())) {
        cout << "create_img_quad: All given images must have same height and widths." << endl;
        return;
    }

    // since all have same widths and heights, pick any to expand the width and heights
    long int quad_height = all_heights.at(0) + all_heights.at(2);

    long int quad_width = all_width.at(1) + all_width.at(3);

    // borrow the header from a value
    Header hdr = header_vector.at(borrow_hdr_at_i);
    // modify the height and width of this header
    hdr.imageHeight = static_cast<short>(quad_height);
    hdr.imageWidth = static_cast<short>(quad_width);
    // then push it in a header vector
    header_vector.push_back(hdr);

    vector<Pixel> temp{};

    // for the bottom portion of the quadrant
    for (int i = 0; i < static_cast<int>(all_heights.at(2)); i++) {

        // iterate until a width of bottle left
        for (int c = 0, p_channel = (i * all_width.at(2)); c < static_cast<int>(all_width.at(2)); c++) {
            //push pixel channels
            temp.push_back(pixel_map.at(bott_left).at(p_channel + c));
        }

        // iterate until a width of bottom right
        for (int c = 0, p_channel = (i * all_width.at(3)); c < static_cast<int>(all_width.at(3)); c++) {
            //push pixel channels
            temp.push_back(pixel_map.at(bott_right).at(p_channel + c));
        }

    }

    // for the top portion of the quadrant
    for (int i = 0; i < static_cast<int>(all_heights.at(0)); i++) {
        // iterate until a width of top left
        for (int c = 0, p_channel = (i * all_width.at(0)); c < static_cast<int>(all_width.at(0)); c++) {
            //push pixel channels
            temp.push_back(pixel_map.at(top_left).at(p_channel + c));
        }
        // iterate until a width of top right
        for (int c = 0, p_channel = (i * all_width.at(1)); c < static_cast<int>(all_width.at(1)); c++) {
            //push pixel channels 
            temp.push_back(pixel_map.at(top_right).at(p_channel + c));
        }

    }

    // insert it as a new image
    pixel_map.insert(make_pair(img_hdr_key, temp));

    stored_images.push_back(string(to_string(img_hdr_key) + ": " + img_name));

    img_hdr_key++;

}

// return two largest heights from a given vector
vector<short> Image::return_two_lrg_vals(vector<short>& short_vector_vals) {

    vector<short> two_large{};

    if (short_vector_vals.size() != 0) {

        short first_max_height = short_vector_vals.at(0);
        short sec_max_height = short_vector_vals.at(1);

        for (int i = 2; i < static_cast<int>(short_vector_vals.size()); i++) {

            if (short_vector_vals.at(i) >= first_max_height) {
                sec_max_height = first_max_height;
                first_max_height = short_vector_vals.at(i);
            }
            else if (short_vector_vals.at(i) > sec_max_height) {
                sec_max_height = short_vector_vals.at(i);
            }
        }

        two_large.push_back(first_max_height);
        two_large.push_back(sec_max_height);

    }

    return (two_large);
}

/*========================================= Header stuff ===========================================================*/

// counter for keep the track of input, output images
int Image::img_hdr_key = 0;

/*========================================= Pixel stuff ===========================================================*/

// randomizes the pixels values and changes the rgb values in all the stored images
void Image::randomize_pxls_in_all() {

    // iterators over all the pixels and randomly assigns a value to each pixels. For fun.
    int myseed = 700;

    default_random_engine rng(myseed);
    uniform_int_distribution<int> rng_dist(0, 255);

    unsigned char rand_red = static_cast<unsigned char>(rng_dist(rng));
    unsigned char rand_green = static_cast<unsigned char>(rng_dist(rng));
    unsigned char rand_blue = static_cast<unsigned char>(rng_dist(rng));

    for (map<int, vector<Pixel>>::iterator itr = pixel_map.begin(); itr != pixel_map.end(); itr++) {

        vector<Pixel>& pxl = itr->second;

        for (Pixel& p : pxl) {
            p.blue = rand_blue;
            p.green = rand_green;
            p.red = rand_red;
        }

    }
}

// add to the blue values only
void Image::add_to_blue_pxls(int& img_ind, unsigned char increase_by) {

    if (pixel_map.size() <= 0) {
        cout << "add_to_blue_pxls: No images. Read at least one image." << endl;
        return;
    }

    Pixel pxl;
    vector<Pixel> temp{};

    vector<Pixel>& pxls_vector = pixel_map.at(img_ind);

    for (Pixel& p : pxls_vector) {
        // clamp the values if over-under flow
        p.blue = clamp(p.blue, increase_by, "adding");
    }

}

// add to the green values only
void Image::add_to_green_pxls(int& img_ind, unsigned char increase_by) {

    if (pixel_map.size() <= 0) {
        cout << "add_to_green_pxls: No images. Read at least one image." << endl;
        return;
    }
    Pixel pxl;
    vector<Pixel> temp{};

    vector<Pixel>& pxls_vector = pixel_map.at(img_ind);

    for (Pixel& p : pxls_vector) {
        p.green = clamp(p.green, increase_by, "adding");
    }

}

// add to the red values only
void Image::add_to_red_pxls(int& img_ind, unsigned char increase_by) {


    if (pixel_map.size() <= 0) {
        cout << "add_to_red_pxls: No images. Read at least one image." << endl;
        return;
    }

    Pixel pxl;
    vector<Pixel> temp{};

    vector<Pixel>& pxls_vector = pixel_map.at(img_ind);

    for (Pixel& p : pxls_vector) {

        p.red = clamp(p.red, increase_by, "adding");

    }

}

// scale the blue values only
void Image::scale_blue_pxls(int& img_ind, unsigned char scale_by) {

    if (pixel_map.size() <= 0) {
        cout << "scale_blue_pxls: No images. Read at least one image." << endl;
        return;
    }

    Pixel pxl;
    vector<Pixel> temp{};

    vector<Pixel>& pxls_vector = pixel_map.at(img_ind);

    for (Pixel& p : pxls_vector) {
        // clamp the pixel values with scaling mode
        p.blue = clamp(p.blue, scale_by, "scaling");

    }

}

// scale the green values only
void Image::scale_green_pxls(int& img_ind, unsigned char scale_by) {

    if (pixel_map.size() <= 0) {
        cout << "scale_green_pxls: No images. Read at least one image." << endl;
        return;
    }

    Pixel pxl;
    vector<Pixel> temp{};

    vector<Pixel>& pxls_vector = pixel_map.at(img_ind);

    for (Pixel& p : pxls_vector) {

        p.green = clamp(p.green, scale_by, "scaling");

    }

}

// scale the red values only
void Image::scale_red_pxls(int& img_ind, unsigned char scale_by) {

    if (pixel_map.size() <= 0) {
        cout << "scale_red_pxls: No images. Read at least one image." << endl;
        return;
    }

    Pixel pxl;
    vector<Pixel> temp{};

    vector<Pixel>& pxls_vector = pixel_map.at(img_ind);

    for (Pixel& p : pxls_vector) {

        p.red = clamp(p.red, scale_by, "scaling");

    }

}


// give size of pixel channel stored inside a pixel vector
long int Image::pixel_channel_size(int& index) {

    return (header_vector.at(index).imageHeight * header_vector.at(index).imageWidth);
}

// noarmalize the values
unsigned char Image::normalize(unsigned char& a, unsigned char& b) {
    // convert unsigned chars to 0-1 and then add 0.5 precision to get correct un char value
    int n = static_cast<int>((static_cast<int>(a) * static_cast<int>(b) / 255.0f) + 0.5f);
    return (static_cast<unsigned char>(n));
}

// clamp the values
unsigned char Image::clamp(unsigned char& a, unsigned char& b, string which_clamp) {

    int n{ };
    // different mode of clamp
    if (which_clamp.compare("adding") == 0) {
        n = static_cast<int>(b + a);
    }
    else if (which_clamp.compare("subtracting") == 0) {
        n = static_cast<int>(b - a);
    }
    else if (which_clamp.compare("scaling") == 0) {
        n = static_cast<int>(b * a);
    }
    // change the overflow or underflow values
    if (n < 0) {
        n = 0;
    }
    else if (n > 255) {
        n = 255;
    }
    // return the unsigned char value
    return (static_cast<unsigned char>(n));
}

// factor based normalization the values
unsigned char Image::factored_normalize(unsigned char& a, unsigned char& b, bool to_factor, float fctr) {

    int n{};
    // if factoring then convert the unsig. char to ints
    if (to_factor == true) {
        int int_a = static_cast<int>(a);
        int int_b = static_cast<int>(b);
        //then apply the overlay formula 
        n = static_cast<int>(((1.0f - fctr * ((1.0f - (int_a / 255.0f)) * (1.0f - (int_b / 255.0f)))) * 255.0f) + 0.5f);
    }
    else {
        // otherwise just multiply the pixels vals by 2 if the base layer is darker
        n = static_cast<int>((2.0f * (static_cast<float>(a) * static_cast<float>(b) / 255.0f) + 0.5f));
    }

    return (static_cast<unsigned char>(n));
}