package com.sourcico.cloudonixndk.models;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class IpAddress {
    @Expose
    @SerializedName("type")
    public String type;
    @Expose
    @SerializedName("nat")
    public boolean nat;
    @Expose
    @SerializedName("address")
    public String address;

    public IpAddress() {
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getAddress() {
        return address;
    }

    // default for request
    public void setAddress(String address) {
        this.address = address;
    }

    public IpAddress(String address) {
        this.address = address;
    }

    public IpAddress(String type, String address) {
        this.type = type;
        this.address = address;
    }

    public IpAddress(String type, boolean nat, String address) {
        this.type = type;
        this.nat = nat;
        this.address = address;
    }

    public boolean isNat() {
        return nat;
    }

    public void setNat(boolean nat) {
        this.nat = nat;
    }

    @Override
    public String toString() {
        return "Address { " +
                ", nat=" + nat +
                ", address='" + address + '\'' +
                '}';
    }
}
