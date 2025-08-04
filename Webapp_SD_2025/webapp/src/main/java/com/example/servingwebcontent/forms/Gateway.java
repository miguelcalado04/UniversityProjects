package com.example.servingwebcontent.forms;
import org.springframework.stereotype.Component;

//package search;

import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.*;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.stream.Collectors;

@Component
public class Gateway extends UnicastRemoteObject implements Index {
    
    private List<Index> storageBarrels;
    private Callback callback;
    private Random random;
    private ConcurrentHashMap<String, Integer> searchCounts = new ConcurrentHashMap<>();

    public Gateway(List<Index> storageBarrels) throws RemoteException {
        this.storageBarrels = storageBarrels;
        this.random = new Random();
    }

    public void registerCallback(Callback callback) throws RemoteException {
        this.callback = callback;
    }

    private Index getRandomStorageBarrel() {
        return storageBarrels.get(random.nextInt(storageBarrels.size()));
    }

    @Override
    public void addToIndex(String word, String url) throws RemoteException {
        getRandomStorageBarrel().addToIndex(word, url);
    }

    @Override
    public void addLink(String fromUrl, String toUrl) throws RemoteException {
        getRandomStorageBarrel().addLink(fromUrl, toUrl);
    }

    @Override
    public List<Map<String, String>> searchLinks(String link, int page) throws RemoteException {
        System.out.println("searchWords chamado no Gateway");
        
        List<Map<String, String>> results = getRandomStorageBarrel().searchLinks(link, page);

        if (callback != null) {
            callback.onSearchComplete(results);
        }
        return results;
    }
    

    @Override
    public List<Map<String, String>> searchWordsWithDetails(List<String> words, int page) throws RemoteException {
        System.out.println("searchWords chamado no Gateway");
        registerSearch(words);
        List<Map<String, String>> results = getRandomStorageBarrel().searchWordsWithDetails(words, page);
        //System.out.println("Resultados da pesquisa: " + results);
        if (callback != null) {
            callback.onSearchComplete(results);
        }
        return results;
    }

    public void registerSearch(List<String> words) {
        String searchKey = String.join(" ", words);
        searchCounts.merge(searchKey, 1, Integer::sum);
        System.out.println("key: " + searchKey);
        System.out.println("search: " + searchCounts);


    }

    public List<Map<String, Object>> getTop10Searches() {
        

        return searchCounts.entrySet().stream()
                .sorted((entry1, entry2) -> entry2.getValue().compareTo(entry1.getValue()))
                .limit(10)
                .map(entry -> {
                    Map<String, Object> map = new HashMap<>();
                    map.put("search", entry.getKey());
                    map.put("count", entry.getValue());
                    return map;
                })
                .collect(Collectors.toList());

        
        
    }

    @Override
    public List<Map<String, Object>> getSystemStats() throws RemoteException {
        
        List<Map<String, Object>> stats = getTop10Searches();

        Map<String, Object> top10Map = new HashMap<>();
        top10Map.put("top10Searches", stats);  
        List<Map<String, Object>> resultStats = new ArrayList<>();
        resultStats.add(top10Map);
        return resultStats;
    }

    
    @Override
    public void savePageData(String url, String title, String bodyText) throws RemoteException {
            try {
                getRandomStorageBarrel().savePageData(url, title, bodyText);
                System.out.println("a guardar");

                return; // salva em apenas um barrel
            } catch (RemoteException e) {
                System.out.println("Erro ao guardar dados no barril. Tentando outro...");
            }
        
        System.out.println("Nenhum barril disponível para guardar os dados da página.");
    }

    @Override
    public PageInfo getPageInfo(String url) throws RemoteException {
        try {
            PageInfo info = getRandomStorageBarrel().getPageInfo(url);
            if (info != null) return info;
        } catch (RemoteException e) {
            System.out.println("Erro ao tentar obter dados da página. Tentando outro...");
        }
        
        return null;
    }


    @Override
    public List<Integer> barr() throws RemoteException{
        List<Integer> indexSizes = new ArrayList<>();

        for (Index barrel : storageBarrels) {
            indexSizes.add(barrel.getIndexSize());
        }

        if (callback != null) {
            callback.barr(indexSizes);
        }

        return indexSizes;
    }

    
    @Override
    public boolean isVisited(String url) throws RemoteException {
        return getRandomStorageBarrel().isVisited(url);
    }

    @Override
    public void markVisited(String url) throws RemoteException {
        getRandomStorageBarrel().markVisited(url);
    }

    public void putNew(String url) throws RemoteException {
        getRandomStorageBarrel().putNew(url);
    }

    @Override
    public String takeNext() throws RemoteException {
        return getRandomStorageBarrel().takeNext();
    }

    public static void main(String[] args) {
        try {
            if (args.length < 2) {
                System.err.println(" Erro: É necessário fornecer pelo menos duas portas como argumentos.");
                return;
            }

            List<Index> barrels = new ArrayList<>();
            for (int i = 0; i < args.length; i++) {
                int port = Integer.parseInt(args[i]);

                System.out.println("Gateway conectada ao barrel na porta:" + port);
                Registry registry = LocateRegistry.getRegistry("localhost", port);
                barrels.add((Index) registry.lookup("StorageBarrel"));
            }

            Gateway gateway = new Gateway(barrels);
            Registry gatewayRegistry = LocateRegistry.createRegistry(8190);
            gatewayRegistry.rebind("Gateway", gateway);

            
        } catch (Exception e) {
           System.out.println("Barril desconectado");
        }
    }

    @Override
    public int getIndexSize() throws RemoteException {
        throw new UnsupportedOperationException("Unimplemented method 'getIndexSize'");
    }

    @Override
    public void logMessage(String message) throws RemoteException {
        throw new UnsupportedOperationException("Unimplemented method 'logMessage'");
    }
}