package com.example.achal.tilt_shift_blur;

import android.graphics.Bitmap;
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
        sigma_far = 4 * 2;
        sigma_near = 4 * 2;
        Bitmap outBmp = Bitmap.createBitmap(input.getWidth(), input.getHeight(), Bitmap.Config.ARGB_8888);
        //cannot write to input Bitmap, since it may be immutable
        //if you try, you may get a java.lang.IllegalStateException
        int kernelRadFar = (int) Math.ceil((2 * sigma_far));
        int kernelRadNear = (int) Math.ceil(2 * sigma_near);
        double[] kernelMatNear, kernelMatFar;
        kernelMatFar = kernelMatrix(kernelRadFar, sigma_far);
        kernelMatNear = kernelMatrix(kernelRadNear, sigma_near);
        int pixelArrayDimention = (input.getHeight()) * (input.getWidth());
        int[] pixels = new int[pixelArrayDimention];
        int[][] pixels2D = new int[(input.getHeight() + kernelMatFar.length - 1)][(input.getWidth() + kernelMatFar.length - 1)];
        int radius_far=kernelMatFar.length/2;
        int radius_near=kernelMatNear.length/2;
        int height=input.getHeight();
        int width=input.getWidth();
        a0=height/5;
        a1=height*2/5;
        a2=height*3/5;
        a3=height*4/5;
        int a1a0=a1-a0;
        int a2a3=a3-a2;
        System.out.println("Array size"+pixels.length+"\tHeight"+height+"\tWidth"+width);
//        Log.d(null, "Pixel length" + pixels2D.length + "\t" + pixels2D[1].length);
        int[] pixelsOut = new int[(width) * (height)];

        input.getPixels(pixels, 0, input.getWidth(), 0, 0, input.getWidth(), input.getHeight());

//      First Transform Starts

        for(int i=0;i<a0;i++){
            for(int j = 0; j < width; j++){
                pixels[i * width + j] = firstTransform(pixels,kernelMatFar,i,j,width,radius_far);
            }
        }
        for(int i=a0;i<(int)a1;i++){
            double newsigmaFar=sigma_far*(a1-i)/a1a0;
            if(newsigmaFar<0.6){

                newsigmaFar=0.6;
            }
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_far));
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaFar);
            int newRadius=newGaussVect.length/2;
            for(int j = newRadius; j < width-newRadius; j++){
                pixels[i * width + j] = firstTransform(pixels,newGaussVect,i,j,width,newRadius);
            }

        }

        for(int i=a2;i<a3;i++){
            double newsigmaNear=sigma_near*(i-a2)/a2a3;
            if(newsigmaNear<0.6){
                newsigmaNear=0.6;
            }
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_near));
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaNear);
            int newRadius=newGaussVect.length/2;
            for(int j = newRadius; j < width-newRadius; j++){
                pixels[i * width + j] = firstTransform(pixels,newGaussVect,i,j,width,newRadius);
            }

        }

        for(int i=a3;i<height;i++){
            for(int j=0;j<width;j++){

                pixels[i * width + j] = firstTransform(pixels,kernelMatNear,i,j,width,radius_near);

            }

        }

//        Second Transform Starts

        for(int i=0;i<(a0);i++) {
            for (int j = 0; j < width ; j++) {
                pixels[i * width + j] = secondTransform(pixels,kernelMatFar,i,j,width,radius_far);
            }

        }


        for(int i=a0;i<a1;i++){
            double newsigmaFar=sigma_far*(a1-i)/a1a0;
            if(newsigmaFar<0.6){
                newsigmaFar=0.6;
            }
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_far));
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaFar);
            int newRadius=newGaussVect.length/2;
            for(int j=0;j<width;j++){
                pixels[i * width + j] = secondTransform(pixels,newGaussVect,i,j,width,newRadius);
            }

        }

        for(int i=a2;i<a3;i++){
            double newsigmaNear=sigma_near*(i-a2)/a2a3;
            if(newsigmaNear<0.6){

                newsigmaNear=0.6;
            }
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_near));
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaNear);
            int newRadius=newGaussVect.length/2;
            for(int j=0;j<width;j++){
                pixels[i * width + j] = secondTransform(pixels,newGaussVect,i,j,width,newRadius);
            }
        }

        for(int i=a3;i<height;i++) {
            for (int j = 0; j < width ; j++) {
                pixels[i * width + j] = secondTransform(pixels,kernelMatNear,i,j,width,radius_near);

            }
        }

        Log.d(null,"Blurring complete !!!!");
        outBmp.setPixels(pixels, 0, input.getWidth(), 0, 0, input.getWidth(), input.getHeight());

        return outBmp;
    }

    //******************************** Java Implementation of Tilt shift blur helper functions******************************



    public static int firstTransform(int[] pixels,double[] kernelMat, int i, int j,int width, int radius){  // Method to calculate First (row wise transform) of bitmap image with gaussian vector

        double R=0.0,G=0.0,B=0.0,A=0.0;
        if(j>=radius && j<width-radius) {
            for (int g_index = -radius; g_index <= radius; g_index++) {
                A += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 3);
                R += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 2);
                G += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 1);
                B += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 0);
            }
        }
        else if(j<radius) {
            for (int g_index = -j,k=0; g_index <=radius; g_index++,k++) {
                A += colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 3);
                R += colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 2);
                G += colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 1);
                B += colorGaussBlur(pixels[i * width+k], kernelMat[radius + g_index], 0);
            }
        }
        else
        {
            for (int g_index = -radius; g_index < width-j; g_index++) {
                A += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 3);
                R += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 2);
                G += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 1);
                B += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 0);
            }
        }



        return ((int) A & 0xff) << 24 | ((int) R & 0xff) << 16 | ((int) G & 0xff) << 8 | ((int) B & 0xff);
    }

    public static int secondTransform(int[] pixels,double[] kernelMat, int i, int j,int width, int radius) {    // Method to calculate Second (column wise transform) of bitmap image with gaussian vector
        double R = 0.0, G = 0.0, B = 0.0, A = 0.0;
        int height = pixels.length / width;
        if (i >= radius && i < height - radius) {
            for (int g_index = -radius; g_index <= radius; g_index++) {
                A += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);
                R += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);
                G += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);
                B += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);

            }
        }
        else if(i<radius){
            for (int g_index = -i; g_index <= radius; g_index++) {
                A += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);
                R += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);
                G += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);
                B += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);
            }

        }
        else{
            for (int g_index = -radius; g_index < height-i; g_index++) {
                A += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);
                R += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);
                G += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);
                B += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);

            }
        }

        return ((int) A & 0xff) << 24 | ((int) R & 0xff) << 16 | ((int) G & 0xff) << 8 | ((int) B & 0xff);
    }


    public static double colorGaussBlur(int pixel,double gauss, int mul){
        int G = (pixel >> (mul*8)) & 0xff;
        double ret =  (G * gauss);
        return ret;
    }


    public static double getGaussian(double k, double var) {
        return ((1 / (2 * Math.PI * Math.pow(var, 2))) * (Math.exp(-1*((Math.pow(k, 2)) / (2 * Math.pow(var, 2))))));
    }

    public static double[] kernelMatrix(int r, float var) {

        double[] matrix = new double[(2 * r + 1)];
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
