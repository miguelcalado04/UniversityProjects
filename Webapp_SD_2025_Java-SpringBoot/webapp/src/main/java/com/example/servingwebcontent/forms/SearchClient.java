package com.example.servingwebcontent.forms;

//package search;
import java.io.Serializable;  
import java.rmi.server.UnicastRemoteObject;

import java.rmi.*;
import java.rmi.registry.*;
import java.util.*;
import java.util.stream.Collectors;

public class SearchClient extends UnicastRemoteObject implements Callback, Serializable {

    private List<Map<String, String>> searchResults = new ArrayList<>();

    public SearchClient() throws RemoteException {
        super(); // Obrigatório para objetos RMI
    }

    @Override
    public void barr(List<Integer> results) throws RemoteException {
      
    }

    @Override
    public void onSearchResults(List<Map<String, String>> results) throws RemoteException {
        
        System.out.println("Resultados da pesquisa: " + results);
    }

    @Override
    public void tops(Map<String, Object> results) throws RemoteException{
        
    }

    @Override
    public Map<String, Object> getSystemStats() throws RemoteException {
        return new HashMap<>(); // Retorna uma lista vazia
    }

    


   
    @Override
    public void onSearchComplete(List<Map<String, String>> results) throws RemoteException {
        System.out.println("Pesquisa concluída! Resultados:");

        // Guarda os resultados para acesso posterior (ex: via interface web)
        this.searchResults = results;

        for (Map<String, String> result : results) {
            System.out.println("Título: " + result.get("title"));
            System.out.println("URL: " + result.get("url"));
            System.out.println("Citação: " + result.get("Excerto"));
            System.out.println("Links associados: " + result.get("relatedUrls"));
            System.out.println();
        }
    }

    public List<Map<String, String>> getSearchResults() {
        return searchResults;
    }

    @Override
    public void onError(String message) throws RemoteException {
        System.out.println("Erro: " + message);
    }

    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.getRegistry("localhost", 8190); // Conectando ao Gateway
            Index gateway = (Index) registry.lookup("Gateway");

            // Criando uma instância do cliente que implementa Callback
            SearchClient client = new SearchClient();

            // Registrando o cliente no Gateway para receber callbacks
            gateway.registerCallback(client);

            Scanner scanner = new Scanner(System.in);
            while (true) {
                System.out.println("Menu:");
                System.out.println("1 - Pesquisar palavras");
                System.out.println("2 - Indexar nova URL");
                System.out.println("3 - estatisticas ");
                System.out.println("4 - Sair");
                System.out.print("Escolha uma opção: ");
                
                String choice = scanner.nextLine();
                if (choice.equals("4")) break;
                
                switch (choice) {
                    case "1":
                        System.out.println("Digite as palavras para pesquisar:");
                        String input = scanner.nextLine();
                        List<String> words = Arrays.asList(input.split("\\s+"));
                        
                        int page = 1;
                        int a = 1;
                        gateway.searchWordsWithDetails(words, page);  // Inicia a pesquisa
                        while(a!=0){
                            System.out.println("Insira 1 se pretende passar para a proxima pagina (0 para terminar):");
                            a = Integer.parseInt(scanner.nextLine());
                            page = page + a;
                            if(a==0){break;}
                            gateway.searchWordsWithDetails(words, page); 
                        }
                        break;
                    
                    case "2":
                        while (true) {
                            System.out.println("Digite a URL para indexar ou 'sair':");
                            String url = scanner.nextLine();
                            if (url.equalsIgnoreCase("sair")) {
                                break;
                            }
                            gateway.putNew(url); // Indexa a URL
                            System.out.println("URL indexada com sucesso!");
                        }
                        break;
    
                        case "3":
                            while (true) {
                                try {
                                    System.out.println("Top 10 Pesquisas:");

                                    // Obtém as estatísticas do gateway
                                    List<Map<String, Object>> topSearchesList = gateway.getSystemStats(); 

                                    // A lista de resultados que vamos preencher com os 10 mais pesquisados
                                    List<Map<String, Object>> top10Searches = new ArrayList<>();

                                    // Itera sobre a lista de mapas (topSearchesList)
                                    for (Map<String, Object> topSearches : topSearchesList) {
                                        // Para cada mapa, obtém a lista associada à chave "top10Searches"
                                        List<Map<String, Object>> searches = (List<Map<String, Object>>) topSearches.get("top10Searches");

                                        // Ordena os itens dessa lista com base na contagem (supõe-se que cada item tem uma chave 'count')
                                        List<Map<String, Object>> sortedSearches = searches.stream()
                                            .sorted((entry1, entry2) -> {
                                                Integer count1 = (Integer) entry1.get("count");
                                                Integer count2 = (Integer) entry2.get("count");
                                                return count2.compareTo(count1); // Ordena em ordem decrescente
                                            })
                                            .limit(10) // Limita para os 10 primeiros
                                            .collect(Collectors.toList());

                                        // Adiciona os resultados ordenados à lista final
                                        top10Searches.addAll(sortedSearches);
                                    }

                                    // Exibe as 10 pesquisas mais frequentes
                                    for (Map<String, Object> search : top10Searches) {
                                        String searchTerm = (String) search.get("search");
                                        Integer count = (Integer) search.get("count");
                                        System.out.println("Palavra: " + searchTerm + " - Número de vezes: " + count);
                                    }

                                    break;               
                                } catch (Exception e) {
                                    System.out.println("Sem estatisticas");
                                    break;
                                }
                            }
                            break;
                              
                    case "4":
                    while (true) {
                        System.out.println("Digite a URL para indexar ou 'sair':");
                        String url = scanner.nextLine();
                        if (url.equalsIgnoreCase("sair")) {
                            break;
                        }
                        gateway.putNew(url); // Indexa a URL
                        System.out.println("URL indexada com sucesso!");
                    }
                    break;
                    
                    default:
                        System.out.println("Opção inválida. Tente novamente.");
                        break;
                }
            }
            scanner.close();
        } catch (Exception e) {

            //tentar conectar a gateway
            System.out.println("Erro ao iniciar o servidor: ");
        }
    }


    
}