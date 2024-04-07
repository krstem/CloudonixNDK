package com.sourcico.cloudonixndk;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.sourcico.cloudonixndk.databinding.ActivityMainBinding;
import com.sourcico.cloudonixndk.models.IpAddress;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    // Used to load the 'cloudonixndk' library on application startup.
    static {
        System.loadLibrary("cloudonixndk");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        binding.btnSampleIpCall.setOnClickListener(v -> {
            binding.progressCircular.setVisibility(View.VISIBLE);
            binding.txtPleaseWait.setVisibility(View.VISIBLE);
            binding.btnSampleIpCall.setVisibility(View.GONE);

            String ipAddress = ipAddressFromJNI();
            Call<IpAddress> call = App.apiService.checkAddress(new IpAddress(ipAddress));
            call.enqueue(new Callback<IpAddress>() {
                @Override
                public void onResponse(Call<IpAddress> call, Response<IpAddress> response) {
                    if (!response.isSuccessful()) {
                        // Handle the error scenario here
                        binding.progressCircular.setVisibility(View.GONE);
                        binding.txtPleaseWait.setVisibility(View.GONE);
                        binding.btnSampleIpCall.setVisibility(View.VISIBLE);
                        return;
                    }
                    IpAddress data = response.body();
                    assert data != null;
                    if (data.isNat()) {
                        if (ipAddress.contains("%")){
                            binding.sampleTextBackFromC.setText(String.format("IP: %s", ipAddress.split("%")[0]));

                        } else {
                            binding.sampleTextBackFromC.setText(String.format("IP: %s", ipAddress));
                        }
                        binding.progressCircular.setVisibility(View.GONE);

                    }
                    Toast.makeText(MainActivity.this, "Data from server: " + data.toString(), Toast.LENGTH_SHORT).show();
                }

                @Override
                public void onFailure(Call<IpAddress> call, Throwable throwable) {
                    Log.w(TAG, "onFailure: ", throwable );
                }
            });
        });


//        binding.sampleTextBackFromC.setText(process());
    }

    /**
     * A native method that is implemented by the 'cloudonixndk' native library,
     * which is packaged with this application.
     */
    public native String ipAddressFromJNI();

    public native String process();

    public String processFromJNI(){
        return "Process in JAVA";
    };
}