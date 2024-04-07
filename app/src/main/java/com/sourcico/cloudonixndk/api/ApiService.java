package com.sourcico.cloudonixndk.api;

import com.sourcico.cloudonixndk.models.IpAddress;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.POST;

public interface ApiService {

    @POST("aws")
    Call<IpAddress> checkAddress(@Body IpAddress ipAddress);
}
