package com.example.minnesotaproject;

import static java.lang.Integer.parseInt;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;


import java.util.ArrayList;

//////-----@ SAURABH KUMAR ----------////////


public class MainActivity extends AppCompatActivity {
    private ECGConfig ecgConfig;
    public static native int ResetMinnesota();
    public static native int FilterLeads(double ecgSample, int leadChannel, int leadSize, int stressORecg);
    public static native double[] getRRIntervalArray();
          static {
            System.loadLibrary("dll1");
            }

    @Override
    protected void onCreate(Bundle savedInstanceState){

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ecgConfig = ECGConfig.getInstance();

        try {
            calculateMinnesota();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    private void calculateMinnesota() {

        int result = 0;
        ArrayList<Double> firstValueList = new ArrayList<>();
        int TotalSize_lead1;
        int TotalSize_lead2;
        int TotalSize_lead3;

        ResetMinnesota();

//            for(int e1=1;e1<=4;e1++) {
        //ecgConfig.ECGCOUNT
        if (ecgConfig.ECGCOUNT == 1) {
            TotalSize_lead1 = ecgConfig.lead1.size();                         // ecgConfig.lead1.size();
            System.out.println("Lead1 TotalSize is: " + TotalSize_lead1);
            for (int i = 0; i < TotalSize_lead1; i++) {
                result = FilterLeads(ecgConfig.lead1.get(i), ecgConfig.ECGCOUNT, TotalSize_lead1, 0);

            }
            System.out.println("Lead 1 Value is: " + result);
            ecgConfig.minneSotaArrayLeadI.add(result);
            LeadValueItem leadValueItemLead1 = new LeadValueItem();
            leadValueItemLead1.setLeadName("Lead 1");
            double[] rrIntervalArray = getRRIntervalArray();
            for (double d : rrIntervalArray) {
                firstValueList.add(d);
            }
            leadValueItemLead1.setValue(firstValueList);
            ecgConfig.arrayRRInt.add(leadValueItemLead1);

        if(ecgConfig.ECGCOUNT1==2){
            TotalSize_lead2 = ecgConfig.lead2.size();
            System.out.println("Lead2 TotalSize is: " + TotalSize_lead2);
            for (int i = 0; i < TotalSize_lead2; i++) {
                result = FilterLeads(ecgConfig.lead2.get(i), ecgConfig.ECGCOUNT1, TotalSize_lead2, 0);
            }
            ecgConfig.minneSotaArrayLeadII.add(result);
            firstValueList.clear();
            LeadValueItem leadValueItemLead2 = new LeadValueItem();
            leadValueItemLead2.setLeadName("Lead 2");
            double[] rrIntervalArrayLead2 = getRRIntervalArray();
            for (double d : rrIntervalArrayLead2) {
                firstValueList.add(d);
            }
            leadValueItemLead2.setValue(firstValueList);
            ecgConfig.arrayRRInt.add(leadValueItemLead2);
        }
//        if(ecgConfig.ECGCOUNT2==3){
//
//            TotalSize_lead3 = ecgConfig.lead3.size();
//            System.out.println("Lead3 TotalSize is: " + TotalSize_lead3);
//            for (int i = 0; i < TotalSize_lead3; i++) {
//                lead3_result = FilterLeads(ecgConfig.lead3.get(i), ecgConfig.ECGCOUNT2, TotalSize_lead3, 0);
//
//            }
//            System.out.println("Main Lead 3 Value is: " + lead3_result);
//            System.out.println(ecgConfig.minneSotaArrayLeadIII.add(lead3_result));
//            ecgConfig.minneSotaArrayLeadIII.add(lead3_result);
//            firstValueList.clear();
//        }



            //                    case 2:
//                        TotalSize = ecgConfig.lead2.size();
//                        System.out.println("Lead2 TotalSize is: " + TotalSize);
//                        for (int i = 0; i < TotalSize; i++) {
//                            lead2_result = FilterLeads(ecgConfig.lead2.get(i), 2, TotalSize, 0);
//                        }
//                        System.out.println("Main Lead 2 Value is: " + lead2_result);
//                        ecgConfig.minneSotaArrayLeadII.add(lead2_result);
//                        firstValueList.clear();
//                        LeadValueItem leadValueItemLead2 = new LeadValueItem();
//                        leadValueItemLead2.setLeadName("Lead 2");
//                        double[] rrIntervalArrayLead2 = getRRIntervalArray();
//                        for (double d : rrIntervalArrayLead2) {
//                            firstValueList.add(d);
//                        }
//                        leadValueItemLead2.setValue(firstValueList);
//                        ecgConfig.arrayRRInt.add(leadValueItemLead2);
//                        break;

//                    case 3:
//                        TotalSize = ecgConfig.lead3.size();
//                        System.out.println("Lead3 TotalSize is: " + TotalSize);
//                        for (int i = 0; i < TotalSize; i++) {
//                            lead3_result = FilterLeads(ecgConfig.lead3.get(i), 3, TotalSize, 0);
//
//                        }
//                        System.out.println("Main Lead 3 Value is: " + lead3_result);
//                        System.out.println(ecgConfig.minneSotaArrayLeadIII.add(lead3_result));
//                        ecgConfig.minneSotaArrayLeadIII.add(lead3_result);
//                        firstValueList.clear();
//                    case 4:
//                        TotalSize = ecgConfig.v1.size();
//                        System.out.println("V1 TotalSize is: " + TotalSize);
//                        for (int i = 0; i < TotalSize; i++) {
//                            v1_result = FilterLeads(ecgConfig.v1.get(i), 4, TotalSize, 0);
//                        }
//                        System.out.println("Main V1 Value is: " + v1_result);
//                        ecgConfig.minneSotaArrayLeadv1.add(v1_result);
//                        firstValueList.clear();
//                        LeadValueItem leadValueItemLeadV1 = new LeadValueItem();
//                        leadValueItemLeadV1.setLeadName("V1");
//                        double[] rrIntervalArrayLeadV1 = getRRIntervalArray();
//                        for (double d : rrIntervalArrayLeadV1) {
//                            firstValueList.add(d);
//                        }
//                        leadValueItemLeadV1.setValue(firstValueList);
//                        ecgConfig.arrayRRInt.add(leadValueItemLeadV1);
//                        break;
        } else {
            throw new IllegalStateException("Unexpected value: " + ecgConfig.ECGCOUNT);
//            case 4:
//                TotalSize = ecgConfig.v2.size();
//                for (int i = 0; i < TotalSize; i++) {
//                    result = FilterLeads(ecgConfig.v2.get(i), ecgConfig.ECGCOUNT, TotalSize, 0);
//                }
//                ecgConfig.minneSotaArrayLeadv2.add(result);
//                firstValueList.clear();
//                LeadValueItem leadValueItemLeadV2 = new LeadValueItem();
//                leadValueItemLeadV2.setLeadName("V2");
//                double[] rrIntervalArrayLeadV2 = getRRIntervalArray();
//                for (double d : rrIntervalArrayLeadV2) {
//                    firstValueList.add(d);
//                }
//                leadValueItemLeadV2.setValue(firstValueList);
//                ecgConfig.arrayRRInt.add(leadValueItemLeadV2);
//                break;
//            case 5:
//                TotalSize = ecgConfig.v3.size();
//                for (int i = 0; i < TotalSize; i++) {
//                    result = FilterLeads(ecgConfig.v3.get(i), ecgConfig.ECGCOUNT, TotalSize, 0);
//                }
//                ecgConfig.minneSotaArrayV3.add(result);
//                firstValueList.clear();
//                LeadValueItem leadValueItemLeadV3 = new LeadValueItem();
//                leadValueItemLeadV3.setLeadName("V3");
//                double[] rrIntervalArrayLeadV3 = getRRIntervalArray();
//                for (double d : rrIntervalArrayLeadV3) {
//                    firstValueList.add(d);
//                }
//                leadValueItemLeadV3.setValue(firstValueList);
//                ecgConfig.arrayRRInt.add(leadValueItemLeadV3);
//                break;
//            case 6:
//                TotalSize = ecgConfig.v4.size();
//                for (int i = 0; i < TotalSize; i++) {
//                    result = FilterLeads(ecgConfig.v4.get(i), ecgConfig.ECGCOUNT, TotalSize, 0);
//                }
//                ecgConfig.minneSotaArrayV4.add(result);
//                firstValueList.clear();
//                LeadValueItem leadValueItemLeadV4 = new LeadValueItem();
//                leadValueItemLeadV4.setLeadName("V4");
//                double[] rrIntervalArrayLeadV4 = getRRIntervalArray();
//                for (double d : rrIntervalArrayLeadV4) {
//                    firstValueList.add(d);
//                }
//                leadValueItemLeadV4.setValue(firstValueList);
//                ecgConfig.arrayRRInt.add(leadValueItemLeadV4);
//                break;
//            case 7:
//                TotalSize = ecgConfig.v5.size();
//                for (int i = 0; i < TotalSize; i++) {
//                    result = FilterLeads(ecgConfig.v5.get(i), ecgConfig.ECGCOUNT, TotalSize, 0);
//                }
//                ecgConfig.minneSotaArrayV5.add(result);
//                firstValueList.clear();
//                LeadValueItem leadValueItemLeadV5 = new LeadValueItem();
//                leadValueItemLeadV5.setLeadName("V5");
//                double[] rrIntervalArrayLeadV5 = getRRIntervalArray();
//                for (double d : rrIntervalArrayLeadV5) {
//                    firstValueList.add(d);
//                }
//                leadValueItemLeadV5.setValue(firstValueList);
//                ecgConfig.arrayRRInt.add(leadValueItemLeadV5);
//                break;
//            case 8:
//                TotalSize = ecgConfig.v6.size();
//                for (int i = 0; i < TotalSize; i++) {
//                    result = FilterLeads(ecgConfig.v6.get(i), ecgConfig.ECGCOUNT, TotalSize, 0);
//                }
//                ecgConfig.minneSotaArrayV6.add(result);
//                firstValueList.clear();
//                LeadValueItem leadValueItemLeadV6 = new LeadValueItem();
//                leadValueItemLeadV6.setLeadName("V6");
//                double[] rrIntervalArrayLeadV6 = getRRIntervalArray();
//                for (double d : rrIntervalArrayLeadV6) {
//                    firstValueList.add(d);
//                }
//                leadValueItemLeadV6.setValue(firstValueList);
//                ecgConfig.arrayRRInt.add(leadValueItemLeadV6);
//                break;
        }
//            }
            }

    }
//}