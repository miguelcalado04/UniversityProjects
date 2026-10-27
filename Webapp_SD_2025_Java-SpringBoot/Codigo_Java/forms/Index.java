package com.example.servingwebcontent.forms;

//package search;

import java.rmi.*;
import java.util.List;
import java.util.Map;
import java.util.Set;

public interface Index extends Remote {
    void putNew(String url) throws RemoteException;
    String takeNext() throws RemoteException;
    void addToIndex(String word, String url) throws RemoteException;
    void addLink(String fromUrl, String toUrl) throws RemoteException;
    boolean isVisited(String url) throws RemoteException;
    void markVisited(String url) throws RemoteException;
    public int getIndexSize() throws RemoteException;
    public List<Integer> barr() throws RemoteException;
    //public Map<String, Object> getSystemStats() throws RemoteException;
    List<Map<String, Object>> getTop10Searches() throws RemoteException;
    List<Map<String, Object>> getSystemStats() throws RemoteException;

    List<Map<String, String>> searchWordsWithDetails(List<String> words, int page) throws RemoteException;
    List<Map<String, String>>  searchLinks(String words, int page) throws RemoteException;
    void registerCallback(Callback callback) throws RemoteException;
    void logMessage(String message) throws RemoteException;

    void savePageData(String url, String title, String bodyText) throws RemoteException;
    PageInfo getPageInfo(String url) throws RemoteException;
}