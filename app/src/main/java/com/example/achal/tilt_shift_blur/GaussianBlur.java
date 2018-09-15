package com.example.achal.tilt_shift_blur;

import android.graphics.Bitmap;                                                                                          // Importing libraries
import android.util.Log;

public class GaussianBlur {

    private GaussianBlur() {
    }

    private static GaussianBlur gblur;

    public static GaussianBlur getInstance() {
        if (gblur == null)
            gblur = new GaussianBlur();
        return gblur;
    }

    public static Bitmap tiltBlur_java(Bitmap input, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3) {
        Log.d(null, "Running Java #####");
        Log.d(null, "Bitmap input values" + input.getHeight());
        sigma_far = 8 * sigma_far;                                                                                      // Sigma far scaling
        sigma_near = 8 * sigma_near;                                                                                    // sigma near scaling
        Bitmap outBmp = Bitmap.createBitmap(input.getWidth(), input.getHeight(), Bitmap.Config.ARGB_8888);              // creating image bitmap
        int kernelRadFar = (int) Math.ceil((2 * sigma_far));                                                            // Gaussian vector radius for far pixels
        int kernelRadNear = (int) Math.ceil(2 * sigma_near);                                                            // Gaussian vector radius for near pixels
        double[] kernelMatNear, kernelMatFar;
        kernelMatFar = kernelMatrix(kernelRadFar, sigma_far);                                                           // Gaussian Vector far
        kernelMatNear = kernelMatrix(kernelRadNear, sigma_near);                                                        // Gaussian Vector near
        int pixelArrayDimension = (input.getHeight()) * (input.getWidth());                                             // pixel array dimension
        int[] pixels = new int[pixelArrayDimension];                                                                    // pixel array creation and  initialization
        int radius_far=kernelMatFar.length/2;
        int radius_near=kernelMatNear.length/2;
        int height=input.getHeight();                                                                                   //  image height
        int width=input.getWidth();                                                                                     //  image width
        int a1a0=a1-a0;
        int a2a3=a3-a2;
        System.out.println("Array size"+pixels.length+"\tHeight"+height+"\tWidth"+width);

        input.getPixels(pixels, 0, input.getWidth(), 0, 0, input.getWidth(), input.getHeight());            // Storing pixel values into an integer array

//      First Transform Starts(row-wise)
//      q(y, x) = G(-r)*p(y, x-r), + … + G(0)*p(y, x),+ … + G(r)*p(y, x+r)
        for(int i=0;i<a0;i++){                                                                                          // Computing of first transform from 0th row to a0th row of a pixels
            for(int j = 0; j < width; j++){
                pixels[i * width + j] = firstTransform(pixels,kernelMatFar,i,j,width,radius_far);                       // Method call to calculate first transform for each pixel
            }
        }

        for(int i=a0;i<(int)a1;i++){                                                                                    // Computing of first transform from a0th row to a1th row of the image bitmap
            double newsigmaFar=sigma_far*(a1-i)/a1a0;                                                                   // Computing new sigma value based on the current row of the pixel
            if(newsigmaFar<0.6){

                newsigmaFar=0.6;
            }
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_far));                                                      // New radius based on the updated sigma value
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaFar);                                           // New gaussian vector using new radius and sigma value

            int newRadius=newGaussVect.length/2;
            for(int j = newRadius; j < width-newRadius; j++){
                pixels[i * width + j] = firstTransform(pixels,newGaussVect,i,j,width,newRadius);                        // Method call to calculate first transform for each pixel
            }

        }

        for(int i=a2;i<a3;i++){                                                                                         // Computing of first transform from a2th row to a3th row of the image bitmap
            double newsigmaNear=sigma_near*(i-a2)/a2a3;                                                                 // Computing new sigma value based on the current row of the pixel
            if(newsigmaNear<0.6){
                newsigmaNear=0.6;
            }
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaNear);                                          // New gaussian vector using new radius and sigma value
            int newRadius=newGaussVect.length/2;
            for(int j = newRadius; j < width-newRadius; j++){
                pixels[i * width + j] = firstTransform(pixels,newGaussVect,i,j,width,newRadius);                        // Method call to calculate first transform for each pixel
            }

        }

        for(int i=a3;i<height;i++){                                                                                     // Method call to calculate first transform for each pixel from row a3 to height
            for(int j=0;j<width;j++){

                pixels[i * width + j] = firstTransform(pixels,kernelMatNear,i,j,width,radius_near);                     //Method call to calculate first transform for each pixel

            }

        }

//        Second Transform Starts
//        P(y, x) = G(-r)*q(y+r, x), + … + G(0)*q(y, x),+ … + G(r)*q(y+r, x)

        for(int i=0;i<(a0);i++) {                                                                                       // Computing of Second transform from 0th row to a0th row of a pixels
            for (int j = 0; j < width ; j++) {
                pixels[i * width + j] = secondTransform(pixels,kernelMatFar,i,j,width,radius_far);                      // Method call to calculate Second transform for each pixel
            }

        }


    for(int i=a0;i<a1;i++){                                                                                             // Computing of Second transform from a0th row to a1th row of the image bitmap
            double newsigmaFar=sigma_far*(a1-i)/a1a0;                                                                   // Computing new sigma value based on the current row pf the pixel
        if(newsigmaFar<0.6){
                newsigmaFar=0.6;
            }
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_far));                                                      // New radius based on the updated sigma value
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaFar);                                           // New gaussian vector using new radius and sigma value
            int newRadius=newGaussVect.length/2;
            for(int j=0;j<width;j++){
                pixels[i * width + j] = secondTransform(pixels,newGaussVect,i,j,width,newRadius);                       // Method call to calculate Second transform for each pixel
            }

        }

        for(int i=a2;i<a3;i++){                                                                                         // Computing of Second transform from a2th row to a3th of the image bitmap
            double newsigmaNear=sigma_near*(i-a2)/a2a3;                                                                 // Computing new sigma value based on the current row pf the pixel
            if(newsigmaNear<0.6){
                newsigmaNear=0.6;
            }
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_near));                                                     // New radius based on the updated sigma value
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaNear);                                          // New gaussian vector using new radius and sigma value
            int newRadius=newGaussVect.length/2;
            for(int j=0;j<width;j++){
                pixels[i * width + j] = secondTransform(pixels,newGaussVect,i,j,width,newRadius);                       // Method call to calculate Second transform for each pixel
            }
        }

        for(int i=a3;i<height;i++) {                                                                                    // Method call to calculate Second transform for each pixel from row a3 to height
            for (int j = 0; j < width ; j++) {
                pixels[i * width + j] = secondTransform(pixels,kernelMatNear,i,j,width,radius_near);                    //Method call to calculate first transform for each pixel

            }
        }

        Log.d(null,"Blurring complete !!!!");
        outBmp.setPixels(pixels, 0, input.getWidth(), 0, 0, input.getWidth(), input.getHeight());           // Setting transformed bitmap to the output

        return outBmp;
    }

    //******************************** Java Implementation of Tilt shift blur helper methods******************************



    public static int firstTransform(int[] pixels,double[] kernelMat, int i, int j,int width, int radius){               // Method to calculate First (row wise transform) of bitmap image with gaussian vector

        double R=0.0,G=0.0,B=0.0,A=0.0;
        if(j>=radius && j<width-radius) {                                                                                // Loop to calculate row transform for the each pixel on the first index [r,(width - r))
            for (int g_index = -radius; g_index <= radius; g_index++) {
                A += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 3);               //  Color wise transform for Most significant 8 bits
                R += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 2);               //  Color wise transform for red bits
                G += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 1);               //  Color wise transform for green bits
                B += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 0);               //  Color wise transform for blue bits
            }
        }
else if(j<radius) {                                                                                                      // Loop to calculate row transform for the each pixel on the index [0,r)
            for (int g_index = -j,k=0; g_index <=radius; g_index++,k++) {
                A += colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 3);                           //  Color wise transform for Most significant 8 bits
                R += colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 2);                           //  Color wise wise transform for red bits
                G += colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 1);                           //  Color wise wise transform for green bits
                B += colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 0);                           //  Color wise wise transform for blue bits
            }
        }
        else                                                                                                             // Loop to calculate row transform for the each pixel on the index [width-r,width)
        {
            for (int g_index = -radius; g_index < width-j; g_index++) {
                A += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 3);               //  Color wise transform for Most significant 8 bits
                R += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 2);               //  Color wise transform for red bits
                G += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 1);               //  Color wise transform for green bits
                B += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 0);               //  Color wise transform for blue bits
            }
        }



        return ((int) A & 0xff) << 24 | ((int) R & 0xff) << 16 | ((int) G & 0xff) << 8 | ((int) B & 0xff);
    }

    public static int secondTransform(int[] pixels,double[] kernelMat, int i, int j,int width, int radius) {             // Method to calculate Second (column wise transform) of bitmap image with gaussian vector
        double R = 0.0, G = 0.0, B = 0.0, A = 0.0;
        int height = pixels.length / width;
        if (i >= radius && i < height - radius) {                                                                        // Loop to calculate row transform for the each pixel on the first index [r,(width - r))
            for (int g_index = -radius; g_index <= radius; g_index++) {
                A += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);             //  Color wise transform for Most significant 8 bits
                R += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);             //  Color wise transform for red bits
                G += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);             //  Color wise transform for green bits
                B += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);             //  Color wise transform for blue bits

            }
        }
        else if(i<radius){                                                                                               // Loop to calculate row transform for the each pixel on the index [0,r)
            for (int g_index = -i; g_index <= radius; g_index++) {
                A += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);             //  Color wise transform for Most significant 8 bits
                R += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);             //  Color wise transform for red bits
                G += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);             //  Color wise transform for green bits
                B += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);             //  Color wise transform for blue bits
            }

        }
        else{                                                                                                            // Loop to calculate row transform for the each pixel on the index [width-r,width)
            for (int g_index = -radius; g_index < height-i; g_index++) {
                A += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);             //  Color wise transform for Most significant 8 bits
                R += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);             //  Color wise transform for red bits
                G += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);             //  Color wise transform for green bits
                B += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);             //  Color wise transform for blue bits

            }
        }

        return ((int) A & 0xff) << 24 | ((int) R & 0xff) << 16 | ((int) G & 0xff) << 8 | ((int) B & 0xff);                // combining all A,R,G,B transformed values into an integer and returning to the caller
    }


    public static double colorGaussBlur(int pixel,double gauss, int mul){                                                 // Method to calculate transform of each color of an individual pixel
        int G = (pixel >> (mul*8)) & 0xff;
        double ret =  (G * gauss);
        return ret;
    }


    public static double getGaussian(double k, double var) {                                                              // Method calculating gaussian transform for a given value of radius and standard deviation
        return ((1 / (2 * Math.PI * Math.pow(var, 2))) * (Math.exp(-1*((Math.pow(k, 2)) / (2 * Math.pow(var, 2))))));
    }

    public static double[] kernelMatrix(int r, float var) {                                                               // Method to calculate entire gaussian vector

        double[] matrix = new double[(2 * r + 1)];                                                                        // Creating gaussian vector of size 2*r+1
        double total = 0, normTotal = 0;
        for (int i = 0; i < matrix.length; i++) {
            matrix[i] = getGaussian(i - r, var);
            total += matrix[i];
        }
        for (int i = 0; i < matrix.length; i++) {
            matrix[i] = matrix[i] / total;
            normTotal += matrix[i];
        }
        return matrix;
    }
}
