package com.example.minnesotaproject;



import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;

public class LeadValueItem {

    @SerializedName("leadName")
    private String leadName;

    @SerializedName("value")
    private ArrayList<Double> value;

    public String getLeadName() { return leadName;
    }

    public void setLeadName(String leadName) { this.leadName = leadName;
    }

    public ArrayList<Double> getValue() { return value;
    }

    public void setValue(ArrayList<Double> value) { this.value = value;
    }

    @Override
    public String toString() {
        return
                "LeadValueItem{" +
                        "leadName = '" + leadName + '\'' +
                        ",value = '" + value + '\'' +
                        "}";
    }
}