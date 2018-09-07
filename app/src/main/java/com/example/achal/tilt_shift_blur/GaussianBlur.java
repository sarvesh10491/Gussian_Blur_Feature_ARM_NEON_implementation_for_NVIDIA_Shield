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
        sigma_far = 2;
        sigma_near = 1;
        Bitmap outBmp = Bitmap.createBitmap(input.getWidth(), input.getHeight(), Bitmap.Config.ARGB_8888);
        //cannot write to input Bitmap, since it may be immutable
        //if you try, you may get a java.lang.IllegalStateException
        int kernelRadFar = (int) Math.ceil((2 * sigma_far));
        int kernelRadNear = (int) Math.ceil(2 * sigma_near);
        Log.d(null, "Far radius is :" + kernelRadFar);
        double[] kernelMatNear, kernelMatFar;
        kernelMatFar = kernelMatrix(kernelRadFar, sigma_far);
        kernelMatNear = kernelMatrix(kernelRadNear, sigma_near);
//        Log.d(null,"Kernel vector");
//        for(int i=0;i<kernelMatFar.length;i++){
//            Log.d(null,kernelMatFar[i]+"\t");
//        }
        Log.d(null, "\t" + input.getWidth() + "\t" + input.getHeight());
        int pixelArrayDimention = (input.getHeight() + kernelMatFar.length - 1) * (input.getWidth() + kernelMatFar.length - 1);
        int[] pixels = new int[pixelArrayDimention];
        int[][] pixels2D = new int[(input.getHeight() + kernelMatFar.length - 1)][(input.getWidth() + kernelMatFar.length - 1)];

        Log.d(null, "Pixel length" + pixels2D.length + "\t" + pixels2D[1].length);
        int[] pixelsOut = new int[(input.getHeight() + kernelMatFar.length - 1) * (input.getWidth() + kernelMatFar.length - 1)];
        input.getPixels(pixels, 0, input.getWidth(), 0, 0, input.getWidth(), input.getHeight());
        for (int i = 0; i < input.getHeight(); i++) {
            for (int j = 0; j < input.getWidth(); j++) {
                pixels2D[i + kernelMatFar.length / 2][j + kernelMatFar.length / 2] = pixels[i * input.getWidth() + j];


                //               Log.d(null, "\t" + pixels[i * (input.getWidth() + kernelMatFar.length-1) + j]);
            }
        }

      // displayFunc(pixels2D);
        weightVectBlur(pixels2D,kernelMatFar,kernelMatFar.length);
       // displayFunc(pixels2D);
        for (int i = 0; i < input.getHeight(); i++) {
            for (int j = 0; j < input.getWidth(); j++) {
                pixelsOut[i*input.getWidth() + j] = pixels2D[i+kernelMatFar.length / 2][j+kernelMatFar.length / 2];
            }
        }

//        for (int i=0;i<pixelMatDimention;i++){
//            int B = pixels[i]%0xff;
//            int G = (pixels[i]>>8)%0xff;
//            int R = (pixels[i]>>16)%0xff;
//            int A = 0xff;
//            G=0;
//            int color = (A & 0xff) << 24 | (R & 0xff) << 16 | (G & 0xff) << 8 | (B & 0xff);
//            pixelsOut[i]=color;
//        }
        outBmp.setPixels(pixelsOut, 0, input.getWidth(), 0, 0, input.getWidth(), input.getHeight());

        return outBmp;
    }

    public static void displayFunc(int[][] pixels2D) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                System.out.print(pixels2D[i][j] + " ");
            }
            System.out.println();
        }
    }

    public static double getGaussian(double k, double var) {
        return (1 / (2 * Math.PI * Math.pow(var, 2))) * (Math.exp(-(Math.pow(k, 2) / 2 * Math.pow(var, 2))));
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

        System.out.println(normTotal);

        return matrix;
    }

    public static void weightVectBlur(int[][] image_map, double[] gauss_vect, int radius) {
        //static int check1=0;
        for (int index_x = radius / 2; index_x < image_map.length - radius/ 2; index_x++) {
            //static int check2=0;
            for (int index_y = radius / 2; index_y < image_map[index_x].length - radius / 2; index_y++) {
                int[][] pixelMat = new int[radius][radius];
               // static int check3=0;
                for (int i = index_x - radius / 2,k=0; i < index_x + radius / 2; i++,k++) {
                    int sum_row = 0;
                    //static int check4=0;
                    for (int j = index_y - radius / 2,l=0; j < index_y + radius / 2; j++,l++) {
                        pixelMat[k][l]=image_map[i][j];
                        //System.out.print(image_map[i][j]+"_"+i+"_"+j+"\t");
                    }
                   // System.out.println();
                    //q[i] = sum_row;
          //          check3++;
                }
                image_map[index_x][index_y]=transformedPixelValue(pixelMat,gauss_vect);
               // System.out.println("Changed Pixel "+  image_map[index_x][index_y]);
                //displayFunc(image_map);

//                int tmp = 0;
//                for (int i = 0; i < radius; i++) {
//
//                    tmp += transformedPixelValue(q[i], gauss_vect[i]);
//                }
//                image_map[index_x][index_y] = tmp;
            }
            // break;
        }


    }

    public static int transformedPixelValue(int[][] pixel, double[] gauss) {

//        int [] color = new int[4];
//        int B = pixel % 0xff;
//        B = (int) (B * gauss);
//        color[0] = B;
//        int G = (pixel >> 8) % 0xff;
//        G = (int) (G * gauss);
//        color[1] = G;
//        int R = (pixel >> 16) % 0xff;
//        R = (int) (R * gauss);
//        color[2] = R;
        int A = 0xff;
//        color[3] = A;
        int red,green,blue;

        blue=colorTransform(pixel,gauss,0);
        green=colorTransform(pixel,gauss,1);
        red=colorTransform(pixel,gauss,2);
        int color = (A & 0xff) << 24 | (red & 0xff) << 16 | (green & 0xff) << 8 | (blue & 0xff);

        return color;
    }
    public static int colorTransform(int[][] pixel, double[] gauss,int mul){
        int [] colorMat=new int[pixel.length];
        int second_transform=0;
        for(int i=0;i<pixel.length;i++){
            int tmp=0;
            for(int j=0;j<pixel[i].length;j++){
                int pix = (pixel[i][j] >> (mul*8)) % 0xff;
                tmp += (int)(pix*gauss[j]);

            }
            colorMat[i] = tmp;
            second_transform+= colorMat[i]*gauss[i];

        }
        return second_transform;

    }
}
