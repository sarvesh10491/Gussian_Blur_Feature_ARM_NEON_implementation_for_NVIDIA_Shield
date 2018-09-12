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
        sigma_far = (float) 0.9;
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
        Log.d(null,"Kernel vector");
        for(int i=0;i<kernelMatFar.length;i++){
            Log.d(null,kernelMatFar[i]+"\t");
        }
        Log.d(null, "\t" + input.getWidth() + "\t" + input.getHeight());
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
        Log.d(null, "Pixel length" + pixels2D.length + "\t" + pixels2D[1].length);
        int[] pixelsOut = new int[(width) * (height)];

        input.getPixels(pixels, 0, input.getWidth(), 0, 0, input.getWidth(), input.getHeight());

//      First Transform Starts
        for(int i=0;i<a0;i++){
            for(int j = radius_far; j < width-radius_far; j++){
                pixels[i * width + j] = firstTransform(pixels,kernelMatFar,i,j,width,radius_far);
            }

        }
        for(int i=a0;i<a1;i++){
            double newsigmaFar=sigma_far*(a1-i)/a1a0;
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_far));
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaFar);
            int newRadius=newGaussVect.length/2;
            for(int j = newRadius; j < width-newRadius; j++){
                pixels[i * width + j] = firstTransform(pixels,newGaussVect,i,j,width,newRadius);
            }

        }

        for(int i=a2+1;i<a3;i++){
            double newsigmaNear=sigma_near*(i-a2)/a2a3;
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_near));
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaNear);
            int newRadius=newGaussVect.length/2;
            for(int j = newRadius; j < width-newRadius; j++){
                pixels[i * width + j] = firstTransform(pixels,newGaussVect,i,j,width,newRadius);
            }

        }

        for(int i=a3;i<height;i++){
            for(int j=radius_near;j<width-radius_near;j++){

                pixels[i * width + j] = firstTransform(pixels,kernelMatNear,i,j,width,radius_near);

            }

        }

//        Second Transform Starts
        for(int i=radius_far;i<a0;i++) {
            for (int j = 0; j < width ; j++) {
                    pixels[i * width + j] = secondTransform(pixels,kernelMatFar,i,j,width,radius_far);
                }
            }

        for(int i=a0;i<a1;i++){
            double newsigmaFar=sigma_far*(a1-i)/a1a0;
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_far));
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaFar);
            int newRadius=newGaussVect.length/2;
            for(int j=0;j<width;j++){
                pixels[i * width + j] = secondTransform(pixels,newGaussVect,i,j,width,newRadius);
            }

        }

        for(int i=a2+1;i<a3;i++){
            double newsigmaNear=sigma_near*(i-a2)/a2a3;
            double[] newGaussVect;
            int newGaussVectSize=(int) Math.ceil((2 * sigma_near));
            newGaussVect=kernelMatrix(newGaussVectSize, (float) newsigmaNear);
            int newRadius=newGaussVect.length/2;
            for(int j=0;j<width;j++){
                pixels[i * width + j] = secondTransform(pixels,newGaussVect,i,j,width,newRadius);
            }
        }


        for(int i=a3;i<height-radius_near;i++) {
            for (int j = 0; j < width ; j++) {
                pixels[i * width + j] = secondTransform(pixels,kernelMatNear,i,j,width,radius_near);

            }
        }

        Log.d(null,"Blurring complete !!!!");
        outBmp.setPixels(pixels, 0, input.getWidth(), 0, 0, input.getWidth(), input.getHeight());

        return outBmp;
    }

    public static int firstTransform(int[] pixels,double[] kernelMat, int i, int j,int width, int radius){

        double R=0.0,G=0.0,B=0.0,A=0.0;
        for (int g_index = -radius; g_index <= radius; g_index++) {
            A += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 3);
            R += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 2);
            G += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 1);
            B += colorGaussBlur(pixels[i * width + j + g_index], kernelMat[radius + g_index], 0);

        }
        return ((int) A & 0xff) << 24 | ((int) R & 0xff) << 16 | ((int) G & 0xff) << 8 | ((int) B & 0xff);
    }

    public static int secondTransform(int[] pixels,double[] kernelMat, int i, int j,int width, int radius){
        double R=0.0,G=0.0,B=0.0,A=0.0;
        for (int g_index = -radius; g_index <= radius; g_index++) {
              A += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 3);
              R += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 2);
              G += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 1);
              B += colorGaussBlur(pixels[j + (i + g_index) * width], kernelMat[radius + g_index], 0);

            }
        return ((int) A & 0xff) << 24 | ((int) R & 0xff) << 16 | ((int) G & 0xff) << 8 | ((int) B & 0xff);
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

//        System.out.println(normTotal);

        return matrix;
    }

    public static void ybasedBlur(int[][] image_map, double[] gauss_vect, int radius){
        for (int index_x = radius / 2; index_x < (image_map.length - radius/ 2); index_x++) {
            for (int index_y = radius / 2; index_y < (image_map[index_x].length - radius / 2); index_y++) {
                double R=0.0,G=0.0,B=0.0,A=0.0;
                int max=255,min=0;
                for(int i=-radius / 2;i<radius/2;i++){
                    A+=colorGaussBlur(image_map[index_x+i][index_y],gauss_vect[(radius/2)+i],3);
                    R+=colorGaussBlur(image_map[index_x+i][index_y],gauss_vect[(radius/2)+i],2);
                    G+=colorGaussBlur(image_map[index_x+i][index_y],gauss_vect[(radius/2)+i],1);
                    B+=colorGaussBlur(image_map[index_x+i][index_y],gauss_vect[(radius/2)+i],0);

                }
                image_map[index_x][index_y]= ((int)A & 0xff) << 24 | ((int)R & 0xff) << 16 | ((int)G & 0xff) << 8 | ((int)B & 0xff);
//                System.out.print(image_map[index_x][index_y]+"\t");
            }
//            System.out.println();
        }

        for (int index_x = radius / 2; index_x < (image_map.length - radius/ 2); index_x++) {
            for (int index_y = radius / 2; index_y < (image_map[index_x].length - radius / 2); index_y++) {
                double R=0.0,G=0.0,B=0.0,A=0.0;
                int max=255,min=0,color;
                for(int i=-radius / 2;i<radius/2;i++){
                    A+=colorGaussBlur(image_map[index_x+i][index_y],gauss_vect[(radius/2)+i],3);
                    R+=colorGaussBlur(image_map[index_x][index_y+i],gauss_vect[(radius/2)+i],2);
                    G+=colorGaussBlur(image_map[index_x][index_y+i],gauss_vect[(radius/2)+i],1);
                    B+=colorGaussBlur(image_map[index_x][index_y+i],gauss_vect[(radius/2)+i],0);

                }
                image_map[index_x][index_y]= ((int)A & 0xff) << 24 | /*(Math.max(min, Math.min(max,R))*/ ((int)R & 0xff) << 16 | ((int)G & 0xff) << 8 | ((int)B & 0xff);

            }

        }

    }

    public static double colorGaussBlur(int pixel,double gauss, int mul){
        int G = (pixel >> (mul*8)) & 0xff;
        double ret =  (G * gauss);
        return ret;
    }

    public static void weightVectBlur(int[][] image_map, double[] gauss_vect, int radius) {
        //static int check1=0;
        for (int index_x = radius / 2; index_x < (image_map.length - radius/ 2); index_x++) {
            //static int check2=0;
//            System.out.println();
            for (int index_y = radius / 2; index_y < (image_map[index_x].length - radius / 2); index_y++) {
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
//               displayFunc(image_map);
                image_map[index_x][index_y]=transformedPixelValue(pixelMat,gauss_vect);
//                displayFunc(image_map);
//               System.out.print(image_map[index_x][index_y] + " ");

//                break;
//                int tmp = 0;
//                for (int i = 0; i < radius; i++) {
//
//                    tmp += transformedPixelValue(q[i], gauss_vect[i]);
//                }
//                image_map[index_x][index_y] = tmp;
            }
//             break;
//            System.out.println();
        }

    Log.d(null,"Blurring complete !!!!");
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
        int red,green,blue,max=255,min=0;

        blue=Math.max(min, Math.min(max,colorTransform(pixel,gauss,0) ));
        green=Math.max(min, Math.min(max,colorTransform(pixel,gauss,1) ));
        red=Math.max(min, Math.min(max,colorTransform(pixel,gauss,2) ));
        int color = (A & 0xff) << 24 | (red & 0xff) << 16 | (green & 0xff) << 8 | (blue & 0xff);
//        System.out.println("Color ka value"+ color +" Blue"+blue+" Green"+green+" Red"+red);
         return color;
    }
    public static int colorTransform(int[][] pixel, double[] gauss,int mul){
        int [] colorMat=new int[pixel.length];
        int second_transform=0;
        for(int i=0;i<pixel.length;i++){
            int tmp=0;
            for(int j=0;j<pixel[i].length;j++){
                int pix = (pixel[i][j] >> (mul*8)) & 0xff;
//                System.out.print(pix+" ");
                tmp += (int)(pix*gauss[j]);

            }
//            System.out.println("tmp:"+tmp);
            colorMat[i] = tmp;
            second_transform+= colorMat[i]*gauss[i];

        }
//      System.out.println("R/G/B value"+second_transform+" ");
        return second_transform;

    }
}
