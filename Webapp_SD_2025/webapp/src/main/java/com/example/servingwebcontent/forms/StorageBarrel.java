package com.example.servingwebcontent.forms;

//package search;

import java.rmi.*;
import java.rmi.server.*;
import java.rmi.registry.*;
import java.util.concurrent.*;
import java.util.stream.Collectors;
import java.io.*;
import java.util.*;
import org.jsoup.*;
import org.jsoup.nodes.*;
import org.jsoup.select.*;

import org.springframework.stereotype.Component;



@Component
public class StorageBarrel extends UnicastRemoteObject implements Index {
    private static final String BACKUP_FILE = "backup.ser";

    private Set<String> visitedUrls = ConcurrentHashMap.newKeySet();
    private ConcurrentHashMap<String, List<String>> index = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, Set<String>> urlLinks = new ConcurrentHashMap<>();
    private Queue<String> urlQueue = new ConcurrentLinkedQueue<>();
    private volatile boolean shutdownRequested = false;
    private ConcurrentHashMap<String, Integer> searchCounts = new ConcurrentHashMap<>();
    private Callback callback;

    private Map<String, PageInfo> pageInfoMap = new HashMap<>();


    public StorageBarrel() throws RemoteException {
        super();
        loadFromBackup();
        
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("Barril está fechando! Salvando backup...");
            saveToBackup();
        }));
    }

    @Override
    public void logMessage(String message) throws RemoteException {
        processLogMessage(message);
    }

    private void processLogMessage(String message) {
        try {
            if (message.contains(" | Palavras:")) {
                int wordsStartIndex = message.indexOf(" | Palavras:");
                if (wordsStartIndex == -1) return;
                
                String url = message.substring(5, wordsStartIndex).trim();
                String wordsString = message.substring(wordsStartIndex + 12).trim();
                String[] words = wordsString.split(",\\s*");
                
                for (String word : words) {
                    word = word.trim().toLowerCase();
                    index.computeIfAbsent(word, k -> new ArrayList<>()).add(url);
                }
            } else if (message.contains("Links encontrados:")) {
                int linksStartIndex = message.indexOf("Links encontrados: ") + 19;
                int pipeIndex = message.indexOf(" |");
                
                if (pipeIndex == -1 || pipeIndex < 5) return;
                
                String url = message.substring(5, pipeIndex).trim();
                String linksString = message.substring(linksStartIndex).trim();
                
                if (!linksString.isEmpty()) {
                    String[] links = linksString.split(",\\s*");
                    for (String link : links) {
                        link = link.trim();
                        urlLinks.computeIfAbsent(url, k -> new HashSet<>()).add(link);
                    }
                }
            }
        } catch (StringIndexOutOfBoundsException e) {
            System.err.println("Erro ao processar mensagem: " + message);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    
    //--------------------------------------------------------------------------------------

    //------------------------------ ESTATISTICAS ---------------------------------

    // Método para registrar uma pesquisa

    
    //--------------------------------------------------------------------------------

    @Override
    public void putNew(String url) throws RemoteException {
        if (!visitedUrls.contains(url)) {
            urlQueue.add(url);
            

        }
    }

    
    public String takeNext() throws RemoteException {
        return urlQueue.poll();
    }

    @Override
    public void addToIndex(String word, String url) throws RemoteException {
        index.computeIfAbsent(word, k -> new ArrayList<>()).add(url);
    }

    @Override
    public void addLink(String fromUrl, String toUrl) throws RemoteException {
        urlLinks.computeIfAbsent(toUrl, k -> new HashSet<>()).add(fromUrl);
    }

    @Override
    public int getIndexSize() throws RemoteException {
        return index.size();
    }

    @Override
    public boolean isVisited(String url) throws RemoteException {
        return visitedUrls.contains(url);
    }

    @Override
    public void markVisited(String url) throws RemoteException {
        visitedUrls.add(url);
    }

    
   

    @Override
    public List<Map<String, String>> searchWordsWithDetails(List<String> words, int page) throws RemoteException {
        
        System.out.println("ola");

        int start = (page - 1) * 10;
        List<Map<String, String>> results = new ArrayList<>();


        //
        /*for (Map.Entry<String, List<String>> entry : index.entrySet()) {
            String palavra = entry.getKey();
            List<String> urls = entry.getValue();
            System.out.println("PalavraINDEX: " + palavra + " → URLsINDEX: " + urls);
        }*/
        //

        Set<String> matchingUrls = new HashSet<>();
        for (String word : words) {
            List<String> urls = index.getOrDefault(word.toLowerCase(), new ArrayList<>());
            //System.out.println("Palavra: " + word + " → URLs encontradas: " + urls); 
            matchingUrls.addAll(urls);
        }
        //System.out.println("Todas as URLs correspondentes: " + matchingUrls);

        int totalUrls = matchingUrls.size();
        int end = Math.min(start + 10, totalUrls);
        List<String> urlsList = new ArrayList<>(matchingUrls);
        //System.out.println("lista de urls: " + urlsList);


        System.out.println("start " + start);
        System.out.println("end " + end);


        for (int i = start; i < end; i++) {
            //System.out.println("ola2");
            

            String url = urlsList.get(i);
            //String url = "https://pt.wikipedia.org/wiki/Banana";
  
            //System.out.println("url: " + url);
            PageInfo info = pageInfoMap.get(url); // <- buscar do mapa interno já preenchido no savePageData
            //System.out.println("Título: " + info.getTitle());
           

            //System.out.println("info " + info);
            //System.out.println("url11111 " + url);
            
            //if (info == null) continue;

            String title = info.getTitle();
            String bodyText = info.getBodyText();
            String excerto = getExcerto(bodyText, words);
            //System.out.println("url " + url);
            //System.out.println("title " + title);
            Set<String> relatedUrls = urlLinks.getOrDefault(url, new HashSet<>());

            Map<String, String> result = new HashMap<>();
            result.put("title", title);
            result.put("url", url);
            result.put("Excerto", excerto);
            result.put("relatedUrls", String.join("\n -", relatedUrls));
            results.add(result);
        }

        // Ordenar os resultados pelo número de links associados (relatedUrls)
        results.sort((a, b) -> {
            int sizeA = urlLinks.getOrDefault(a.get("url"), new HashSet<>()).size();
            int sizeB = urlLinks.getOrDefault(b.get("url"), new HashSet<>()).size();
            return Integer.compare(sizeB, sizeA);
        });

        return results;
    }


    @Override
    public List<Map<String, String>> searchLinks(String link, int page) throws RemoteException {
        List<Map<String, String>> results = new ArrayList<>();
        Set<String> relatedUrls = urlLinks.getOrDefault(link, new HashSet<>());
        Map<String, String> result = new HashMap<>();
        result.put("relatedUrls", String.join("\n -", relatedUrls));
        results.add(result);

        return results;
    }


    /* 
    @Override
    public List<Map<String, String>> searchWordsWithDetails(List<String> words, int page) throws RemoteException {
        
        List<Map<String, String>> results = new ArrayList<>();
        Map<String, String> result = new HashMap<>();
        result.put("title", "ola");
        results.add(result);
        System.out.println("urllll " + results);


        return results;
    }*/

    /* 
    @Override
    public List<Map<String, String>> searchWordsWithDetails(List<String> words, int page) throws RemoteException {
        //devolver apenas re
        registerSearch(words);
        
        int start = (page - 1) * 10;
        List<Map<String, String>> results = new ArrayList<>();

        Set<String> matchingUrls = new HashSet<>();
        for (String word : words) {
            List<String> urls = index.getOrDefault(word.toLowerCase(), new ArrayList<>());
            matchingUrls.addAll(urls);
        }

        int totalUrls = matchingUrls.size();
        int end = Math.min(start + 10, totalUrls);
        List<String> urlsList = new ArrayList<>(matchingUrls);


        //fazer isto no downloader
        for (int i = start; i < end; i++) {
            String url = urlsList.get(i);
            try {
                System.err.println("ola1 " + url);
                Document doc = Jsoup.connect(url).get();
                System.err.println("urllll " + url);

                String title = doc.title();
                String bodyText = doc.body().text();
                String excerto =  getExcerto(bodyText, words);

               
                Set<String> relatedUrls = urlLinks.getOrDefault(url, new HashSet<>());

                Map<String, String> result = new HashMap<>();
                result.put("title", title);
                result.put("url", url);
                result.put("Excerto", excerto);
                result.put("relatedUrls", String.join("\n -", relatedUrls)); // Convertendo para String
                results.add(result);
            } catch (IOException e) {
                System.err.println("Erro ao acessar URL: " + url);
            }
        }

        // Ordenar os resultados pelo número de links associados (relatedUrls)
        results.sort((a, b) -> {
            int sizeA = urlLinks.getOrDefault(a.get("url"), new HashSet<>()).size();
            int sizeB = urlLinks.getOrDefault(b.get("url"), new HashSet<>()).size();
            return Integer.compare(sizeB, sizeA); // Ordem decrescente (mais links primeiro)
        });

        return results;
    }*/

    private String getExcerto(String text, List<String> words) {
        for (String word : words) {
            int index = text.toLowerCase().indexOf(word.toLowerCase());
            if (index != -1) {
                int start = Math.max(0, index - 50); // Início do excerto
                int end = Math.min(text.length(), index + 50); // Fim do excerto
                
                // Verifica se o intervalo é válido
                if (start < end) {
                    return text.substring(start, end);
                } else {
                    return text.substring(0, Math.min(100, text.length())); // Retorna os primeiros 100 caracteres se o intervalo for inválido
                }
            }
        }
        return text.substring(0, Math.min(100, text.length())); // Caso não encontre a palavra
    }




   public void savePageData(String url, String title, String bodyText) {
        PageInfo info = new PageInfo(title, bodyText);
        pageInfoMap.put(url, info);
        //System.out.println("Dados salvos para: " + url);
        //System.out.println("UUUUUUUU" + info.getTitle());

    }
    @Override
    public PageInfo getPageInfo(String url) throws RemoteException {
        return pageInfoMap.get(url);
    }

    //salva os dados antes do barril encerrar
    private void saveToBackup() {
        try (ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(BACKUP_FILE))) {
            out.writeObject(visitedUrls);
            out.writeObject(index);
            out.writeObject(urlLinks);
            out.writeObject(urlQueue);
            System.out.println(" Backup salvo com sucesso em " + BACKUP_FILE);
        } catch (IOException e) {
            System.err.println(" Erro ao salvar backup: " + e.getMessage());
        }
    }
    
    public void shutdown() {
        shutdownRequested = true;
    }

    public static void main(String[] args) {
        try {
            if (args.length < 1) {
                System.out.println(" Porta não especificada, usando valor padrão 8099.");
                args = new String[] { args.length > 0 ? args[0] : "", "8099" };
            }
    
            int port = Integer.parseInt(args[1]);
    
            StorageBarrel barrel = new StorageBarrel();
            Registry registry = null;
            boolean success = false;
    
            while (!success) {
                try {
                    registry = LocateRegistry.createRegistry(port);
                    registry.rebind("StorageBarrel", barrel);
                    System.out.println(" Storage Barrel ativo na porta " + port + "...");
                    success = true;
                } catch (Exception e) {
                    port++;
                }
            }
    
        } catch (NumberFormatException e) {
            System.err.println(" Erro: Porta fornecida não é válida.Passe um número inteiro para a porta.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @SuppressWarnings("unchecked")
    private void loadFromBackup() {
        System.out.println("Entrou no load");
        File file = new File(BACKUP_FILE);
        if (!file.exists()) {
            System.out.println(" Nenhum backup encontrado. Iniciando vazio.");
            return;
        }
    
        try (ObjectInputStream in = new ObjectInputStream(new FileInputStream(BACKUP_FILE))) {
            visitedUrls = (Set<String>) in.readObject();
            index = (ConcurrentHashMap<String, List<String>>) in.readObject();
            urlLinks = (ConcurrentHashMap<String, Set<String>>) in.readObject();
            urlQueue = (Queue<String>) in.readObject();
            System.out.println(" Backup restaurado com sucesso!");
        } catch (IOException | ClassNotFoundException e) {
            System.err.println(" Erro ao carregar backup: " + e.getMessage());
        }
    
        // Após carregar os dados, tenta apagar o arquivo de backup
        if (file.delete()) {
            System.out.println(" Arquivo de backup apagado com sucesso.");
        } else {
            System.err.println(" Falha ao apagar o arquivo de backup.");
        }
    }

    @Override
    public List<Integer> barr() throws RemoteException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'barr'");
    }

    @Override
    public void registerCallback(Callback callback) throws RemoteException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'registerCallback'");
    }

    @Override
    public List<Map<String, Object>> getTop10Searches() throws RemoteException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getTop10Searches'");
    }

    @Override
    public List<Map<String, Object>> getSystemStats() throws RemoteException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getSystemStats'");
    }

   
}
