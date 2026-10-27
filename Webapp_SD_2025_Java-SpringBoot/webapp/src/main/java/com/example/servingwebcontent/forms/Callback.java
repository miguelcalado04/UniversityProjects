package com.example.servingwebcontent.forms;
//package search;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;
import java.util.Map;
import java.util.Set;

public interface Callback extends Remote {
    void onSearchResults(List<Map<String, String>> results) throws RemoteException;
    void onSearchComplete(List<Map<String, String>> results) throws RemoteException;  
    void tops(Map<String, Object> results) throws RemoteException;
    void barr(List<Integer> results) throws RemoteException;
    void onError(String message) throws RemoteException;  //se der erro
    Map<String, Object> getSystemStats() throws RemoteException;

}