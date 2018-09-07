package com.example.achal.tilt_shift_blur;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.nfc.Tag;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.io.FileNotFoundException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    static MainActivity mainActivity;
    private Spinner LanguageSpinner;
    ArrayAdapter<String> spinAdapter;
    ImageView ivAttachment,ivBlurImage;
    public Bitmap bmp;
    Button blurButton,resetButton;
    private static final int SELECT_PICTURE = 1;
    int lang;
    float a0f, a1f, a2f,a3f, s0f, s1f;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mainActivity = this;
        setupUI();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == SELECT_PICTURE && resultCode == RESULT_OK && null != data) {
            try{
                final Uri uriImage = data.getData();
                final InputStream inputStream = getContentResolver().openInputStream(uriImage);
                final Bitmap imagemap = BitmapFactory.decodeStream(inputStream);
                ivBlurImage.setImageBitmap(imagemap);
                Log.d(null,"Bitmap  imagemap values"+imagemap.getHeight());
                bmp=imagemap.copy(Bitmap.Config.ARGB_8888,true);
                }
            catch (FileNotFoundException e) {
                e.printStackTrace();
                Toast.makeText(this, "Image was not found", Toast.LENGTH_SHORT).show();
            }

        }
    }

    private void setupImg(){
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inPreferredConfig = Bitmap.Config.ARGB_8888;
        bmp = BitmapFactory.decodeResource(this.getResources(), R.id.iv_blur_image,options);
        ivBlurImage.setImageBitmap(bmp);
    }

    private void showFileChooser() {
        Intent intent = new Intent(Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        //sets the select file to all types of files
        intent.setType("image/*");
        //allows to select data and return it
        intent.setAction(Intent.ACTION_GET_CONTENT);
        //starts new activity to select file and return data
        startActivityForResult(Intent.createChooser(intent,"Choose File to Blur.."),SELECT_PICTURE);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    @Override
    public void onClick(View view) {

    }

    private void setupUI(){
        final SeekBar seek0 = (SeekBar)findViewById(R.id.rowSeek0);
        final SeekBar seek1 = (SeekBar)findViewById(R.id.rowSeek1);
        final SeekBar seek2 = (SeekBar)findViewById(R.id.rowSeek2);
        final SeekBar seek3 = (SeekBar)findViewById(R.id.rowSeek3);
        SeekBar seek4 = (SeekBar)findViewById(R.id.rowSeek4);
        SeekBar seek5 = (SeekBar)findViewById(R.id.rowSeek5);
        ivBlurImage = (ImageView) findViewById(R.id.iv_blur_image);
        LanguageSpinner = (Spinner) findViewById(R.id.language_spinner);
        spinAdapter = new ArrayAdapter<>(mainActivity,
                android.R.layout.simple_list_item_1, getResources().getStringArray(R.array.languages));
        spinAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        LanguageSpinner.setAdapter(spinAdapter);

        ivAttachment = (ImageView) findViewById(R.id.ivAttachment);
        ivAttachment.setOnClickListener(this);

        // On Attach button click
        //========================
        ivAttachment.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {
                showFileChooser();
            }
        });


        seek0.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                if (progress<seek1.getProgress()){
                    seek1.setProgress(progress);
                }
                a0f=1-progress/100f;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }

        });

        seek1.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                if (progress>seek0.getProgress()){
                    seek0.setProgress(progress);
                }
                if (progress<seek2.getProgress()){
                    seek2.setProgress(progress);
                }
                a1f=1-progress/100f;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }

        });

        seek2.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                if (progress>seek1.getProgress()){
                    seek1.setProgress(progress);
                }
                if (progress<seek3.getProgress()){
                    seek3.setProgress(progress);
                }
                a2f=1-progress/100f;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }

        });

        seek3.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                if (progress>seek2.getProgress()){
                    seek2.setProgress(progress);
                }
                a3f=1-progress/100f;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }

        });
        seek4.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                s0f=progress/100f;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }

        });
        seek5.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                s1f=progress/100f;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }

        });

        blurButton = (Button) findViewById(R.id.button_blur);
        blurButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                blurButton.setEnabled(false);
                lang = LanguageSpinner.getSelectedItemPosition();
                if (lang==0) {
                    Log.d(null,"Running Java");
                    Log.d(null,"Bitmap bmp values"+bmp.getHeight());
                    Bitmap outbmp = GaussianBlur.tiltBlur_java(bmp, s0f, s1f, (int) (a0f * bmp.getHeight()), (int) (a1f * bmp.getHeight()), (int) (a2f * bmp.getHeight()), (int) (a3f * bmp.getHeight()));
                    ivBlurImage.setImageBitmap(outbmp);
                }
                blurButton.setEnabled(true);

            }
        });

        resetButton = (Button) findViewById(R.id.reset_blur);
        resetButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d(null,"Running Java");
                Log.d(null,"Bitmap bmp values"+bmp.getHeight());
               //Bitmap outbmp = GaussianBlur.tiltBlur_java(bmp, s0f, s1f, (int) (a0f * bmp.getHeight()), (int) (a1f * bmp.getHeight()), (int) (a2f * bmp.getHeight()), (int) (a3f * bmp.getHeight()));
                ivBlurImage.setImageBitmap(bmp);
                //blurButton.setEnabled(true);

            }
        });


    }
}
