import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.InputMismatchException;
import java.util.Scanner;

public class POOFinancialServices{
    private ArrayList<Cliente> listaClientes = new ArrayList<>();
    private ArrayList<Fatura> listaFaturas = new ArrayList<>();
    private boolean importouFaturas = false;
    public static void main(String[] args) {

        POOFinancialServices obj = new POOFinancialServices();

        obj.carregarClientesDeArquivo("clientes.dat");

        Scanner scanner = new Scanner(System.in);
        int opcao = 0;
        do {
            System.out.println("Menu:");
            System.out.println("1. Criar cliente");
            System.out.println("2. Editar cliente");
            System.out.println("3. Listar clientes");
            System.out.println("4. Criar fatura");
            System.out.println("5. Editar fatura");
            System.out.println("6. Listar faturas");
            System.out.println("7. Visualizar fatura");
            System.out.println("8. Importar faturas");
            System.out.println("9. Exportar faturas");
            System.out.println("10. Estatísticas");
            System.out.println("11. Sair");
            System.out.print("Escolha uma opção: ");

            try {
                opcao = scanner.nextInt();
                switch (opcao) {
                    case 1:
                        obj.criarCliente();
                        break;
                    case 2:;
                        obj.editarPreCliente();
                        break;
                    case 3:
                        for(Cliente c :  obj.getListaClientes()) {
                            System.out.println(c);
                        }
                        break;
                    case 4://////////////////////////
                        obj.criarFatura();
                        break;
                    case 5:////////////////////////
                        obj.editarFatura();
                        break;
                    case 6:////////////////////////
                        obj.imprimirFatura();
                        break;
                    case 7:////////////////////////
                        obj.visualizarFaturas();
                        break;
                    case 8:////////////////////////////
                        obj.carregarFaturasDeArquivo("faturas.dat");
                        break;
                    case 9://///////////////////////
                        Scanner scannerr = new Scanner(System.in);
                        if (!obj.isImportouFaturas()) {
                            System.out.println("Aviso: Você não importou faturas do arquivo existente.");
                            System.out.println("Se continuar, o conteúdo atual do arquivo será substituído e os dados anteriores serão perdidos.");
                            System.out.print("Deseja continuar e sobrescrever o arquivo? (s/n): ");
                            String resposta = scannerr.nextLine();

                            // Se o usuário não quiser continuar, a função é encerrada
                            if (!resposta.equalsIgnoreCase("s")) {
                                System.out.println("Operação cancelada pelo usuário. As faturas não foram salvas.");
                                break;
                            }
                        }
                        obj.exportarFaturas("faturas.txt");
                        obj.salvarFaturasEmArquivo("faturas.dat");

                        break;
                    case 10:
                        obj.estatisticasFaturas();
                        break;
                    case 11:
                        System.out.println("Adeus");
                        obj.salvarClientesEmArquivo("clientes.dat");
                        obj.exportarClientes("clientes.txt");
                        break;
                    default:
                        System.out.println("Opção inválida. Tente novamente.");
                }
            } catch (InputMismatchException e) {
                System.out.println("Entrada inválida. Por favor, insira um número.");
                scanner.next();
            }
        } while (opcao != 11);

        scanner.close();
    }

    public void exportarClientes(String nomeArquivo) {
        ArrayList<Cliente> clientes = this.getListaClientes(); // Obtém a lista de clientes
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(nomeArquivo))) {
            for (Cliente cliente : clientes) {

                String linha = cliente.getNome() + "," + cliente.getNif() + "," + cliente.getLocalizacao();
                writer.write(linha);
                writer.newLine();
            }
            System.out.println("Clientes exportados com sucesso para o arquivo: " + nomeArquivo);
        } catch (IOException e) {
            System.err.println("Erro ao exportar clientes: " + e.getMessage());
        }
    }

    public void salvarFaturasEmArquivo(String nomeArquivo) {
        Scanner scanner = new Scanner(System.in);

        // Obtém a lista de faturas
        ArrayList<Fatura> faturas = this.getListaFaturas();

        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(nomeArquivo))) {
            // Salva as faturas no arquivo
            oos.writeObject(faturas);
            System.out.println("Faturas salvas com sucesso no arquivo: " + nomeArquivo);
        } catch (IOException e) {
            System.err.println("Erro ao salvar faturas: " + e.getMessage());
        }
    }


    public void salvarClientesEmArquivo(String nomeArquivo) {
        ArrayList<Cliente> clientes = this.getListaClientes();
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(nomeArquivo))) {
            oos.writeObject(clientes); // Escreve a lista de clientes no arquivo
            System.out.println("Clientes salvos com sucesso no arquivo: " + nomeArquivo);
        } catch (IOException e) {
            System.err.println("Erro ao salvar clientes: " + e.getMessage());
        }
    }

    public void carregarClientesDeArquivo(String nomeArquivo) {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(nomeArquivo))) {
            // Atualiza a lista de clientes com os dados do arquivo
            this.setListaClientes((ArrayList<Cliente>) ois.readObject());
            System.out.println("Clientes carregados com sucesso do arquivo: " + nomeArquivo);
        } catch (FileNotFoundException e) {
            inicializarClientes("clientes.txt");
        } catch (IOException | ClassNotFoundException e) {
            System.err.println("Erro ao carregar clientes: " + e.getMessage());
        }
    }

    public void carregarFaturasDeArquivo(String nomeArquivo) {

        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(nomeArquivo))) {
            // Atualiza a lista de clientes com os dados do arquivo
            this.setListaFaturas((ArrayList<Fatura>) ois.readObject());
            setImportouFaturas(true);
            System.out.println("Faturas carregados com sucesso do arquivo: " + nomeArquivo);
        } catch (FileNotFoundException e) {
            carregarFaturas("Faturas.txt");
        } catch (IOException | ClassNotFoundException e) {
            System.err.println("Erro ao carregar faturas: " + e.getMessage());
        }
    }


    public void estatisticasFaturas() {
        ArrayList<Fatura> listaFaturas = this.getListaFaturas();
        int numeroFaturas = listaFaturas.size();
        int numeroProdutos = 0;
        double valorTotalSemIva = 0;
        double valorTotalIva = 0;
        double valorTotalComIva = 0;

        for (Fatura fatura : listaFaturas) {
            for (Produto produto : fatura.getListaProdutos()) {
                numeroProdutos++;
                double valorSemIva = produto.getValor() * produto.getQuantidade();
                double taxaIva = produto.taxa(fatura.getCliente());
                double valorIva =valorSemIva * taxaIva;

                valorTotalSemIva += valorSemIva;
                valorTotalIva += valorIva;
                valorTotalComIva += (valorSemIva + valorIva);
            }
        }

        System.out.println("\n=== Estatísticas das Faturas ===");
        System.out.println("Número de Faturas: " + numeroFaturas);
        System.out.println("Número de Produtos: " + numeroProdutos);
        System.out.println("Valor Total Sem IVA: " + String.format("%.2f", valorTotalSemIva) + " €");
        System.out.println("Valor Total do IVA: " + String.format("%.2f", valorTotalIva) + " €");
        System.out.println("Valor Total Com IVA: " + String.format("%.2f", valorTotalComIva) + " €");
        System.out.println("===============================");
    }

    public void visualizarFaturas() {
        Scanner scanner = new Scanner(System.in);
        ArrayList<Fatura> listaFaturas = this.getListaFaturas();
        System.out.println("Insira o número da fatura que deseja visualizar:");
        int numeroFatura = scanner.nextInt();

        Fatura faturaEncontrada = null;
        for (Fatura f : listaFaturas) {
            if (f.getNumeroFatura() == numeroFatura) {
                f.visualizarFatura();
                faturaEncontrada = f;
                break;
            }
        }

        if (faturaEncontrada == null) {
            System.out.println("Fatura com o número " + numeroFatura + " não encontrada.");
        }
    }

    public void inicializarClientes( String nomeFicheiro) {
        try (BufferedReader reader = new BufferedReader(new FileReader(nomeFicheiro))) {
            String linha;
            while ((linha = reader.readLine()) != null) {
                String[] dados = linha.split(",");
                this.getListaClientes().add(new Cliente(dados[0], Integer.parseInt(dados[1]), dados[2]));
            }
            System.out.println("Clientes adicionados com sucesso");
        } catch (IOException e) {
            System.out.println("Erro ao ler o ficheiro: " + e.getMessage());
        }
    }


    public void exportarFaturas(String nomeFicheiro) {
        ArrayList<Fatura> listaFaturas = this.getListaFaturas();

        if (listaFaturas.isEmpty()) {
            System.out.println("A lista de faturas está vazia.");
            return;
        }

        try {
            int numeroLinhasExistentes = 0;
            File file = new File(nomeFicheiro);

            // Contar linhas existentes no arquivo
            if (file.exists()) {
                try (BufferedReader reader = new BufferedReader(new FileReader(nomeFicheiro))) {
                    while (reader.readLine() != null) {
                        numeroLinhasExistentes++;
                    }
                }
            }

            // Determinar modo de abertura do arquivo
            boolean isAppend = file.exists() && numeroLinhasExistentes > 0;

            try (BufferedWriter writer = new BufferedWriter(new FileWriter(nomeFicheiro, isAppend))) {
                for (int i = numeroLinhasExistentes; i < listaFaturas.size(); i++) {
                    Fatura fatura = listaFaturas.get(i);

                    // Escrever informações da fatura
                    writer.write(fatura.getNumeroFatura() + ",");
                    writer.write(fatura.getCliente().getNif() + ",");

                    Data data = fatura.getData();
                    writer.write(data.getAno() + "-" + String.format("%02d", data.getMes()) + "-" + String.format("%02d", data.getDia()) + ",");

                    // Exportar produtos diretamente
                    ArrayList<Produto> produtos = fatura.getListaProdutos();
                    for (int j = 0; j < produtos.size(); j++) {
                        Produto produto = produtos.get(j);

                        String produtoExportado = produto.exportar();
                        if (!produtoExportado.isEmpty()) {
                            writer.write(produtoExportado);
                            System.out.println("Produto exportado: " + produtoExportado);
                        }

                        // Adicionar separador
                        if (j < produtos.size() - 1) {
                            writer.write("-");
                        }
                    }

                    // Nova linha para próxima fatura
                    writer.newLine();
                }

                System.out.println("Faturas exportadas com sucesso para o ficheiro " + nomeFicheiro);
            }
        } catch (IOException e) {
            System.out.println("Erro ao exportar as faturas: " + e.getMessage());
        }
    }

    public void criarCliente() {
        ArrayList<Cliente> listaClientes = this.getListaClientes();
        Scanner scanner = new Scanner(System.in);
        String nome;
        Cliente clienteA = new Cliente();
        do {
            System.out.print("Nome (apenas letras): ");
            nome = scanner.next();
            if (!nome.matches("[a-zA-ZÀ-ÿ\\s]+")) {
                System.out.println("Nome inválido! Deve conter apenas letras.");
            }
        } while (!nome.matches("[a-zA-ZÀ-ÿ\\s]+"));

        int nif;
        do {
            System.out.print("NIF (9 dígitos): ");
            String nifStr = scanner.next();
            if (!nifStr.matches("\\d{9}")) {
                System.out.println("NIF inválido! Deve conter exatamente 9 dígitos.");
                continue;
            }
            nif = Integer.parseInt(nifStr);
            if ((procurarClientePeloNif(nif, listaClientes)) == null) {
                break;
            } else {
                System.out.println("NIF inválido! Já existe um cliente com esse nif");
            }
        } while (true);

        String local;
        do {
            System.out.print("Localização (Continente, Madeira ou Açores): ");
            local = scanner.next();
            if (!local.equalsIgnoreCase("Continente") &&
                    !local.equalsIgnoreCase("Madeira") &&
                    !local.equalsIgnoreCase("Açores")) {
                System.out.println("Localização inválida! Escolha entre: Continente, Madeira, Açores.");
            }
        } while (!local.equalsIgnoreCase("Continente") &&
                !local.equalsIgnoreCase("Madeira") &&
                !local.equalsIgnoreCase("Açores"));

        // Criar o novo cliente com os dados validados
        Cliente novoCliente = new Cliente(nome, nif, local);
        listaClientes.add(novoCliente);
        System.out.println("Cliente adicionado com sucesso!");

    }

    public void editarPreCliente() {


        Scanner scanner = new Scanner(System.in);
        int niff = 0;
        boolean nifValido = false;

        do {
            System.out.print("Insira o NIF do cliente que pretende editar (9 dígitos): ");
            String nifStr = scanner.nextLine();
            if (!nifStr.matches("\\d{9}")) {
                System.out.println("NIF inválido! Deve conter exatamente 9 dígitos.");
            } else {
                niff = Integer.parseInt(nifStr);
                nifValido = true;
            }
        } while (!nifValido);
        boolean encontrado = false;
        for (Cliente c : this.getListaClientes()) {
            if (c.getNif() == niff) {
                c.editarCliente();
                return;
            }
        }
            System.out.println("Cliente com o NIF " + niff + " não encontrado.");
    }

    public void imprimirFatura() {
        System.out.println("Lista de Faturas:");
        System.out.println("------------------------------------------------------------");
        System.out.println("Nº Fatura | Cliente | Localização | Nº Produtos | Valor s/ IVA | Valor c/ IVA");
        System.out.println("------------------------------------------------------------");

        ArrayList<Fatura> listaFaturas = this.getListaFaturas();
        for (Fatura f : listaFaturas) {
            f.imprimirFaturas();
        }

    }

    public void criarFatura() {
        Scanner scanner = new Scanner(System.in);

        System.out.println("\n=== Criar Nova Fatura ===");

        // Validação do número da fatura
        int numeroFatura;
        do {
            System.out.print("Insira o número da nova fatura (apenas números): ");
            String numeroFaturaStr = scanner.nextLine();
            if (!numeroFaturaStr.matches("\\d+")) {
                System.out.println("Entrada inválida! O número da fatura deve conter apenas números.");
                continue;
            }

            numeroFatura = Integer.parseInt(numeroFaturaStr);

            // Verifica se o número já existe
            boolean numeroExiste = false;
            for (Fatura f : listaFaturas) {
                if (f.getNumeroFatura() == numeroFatura) {
                    numeroExiste = true;
                    break;
                }
            }

            if (numeroExiste) {
                System.out.println("Erro: Já existe uma fatura com o número " + numeroFatura + ". Insira um número único.");
            } else {
                break;
            }
        } while (true);

        // Seleção do cliente com validação do NIF
        System.out.println("\n--- Lista de Clientes Disponíveis ---");
        for (Cliente cliente : listaClientes) {
            System.out.println(cliente);
        }

        int nifCliente;
        Cliente clienteSelecionado = null;
        do {
            System.out.print("Insira o NIF do cliente (apenas números): ");
            String nifStr = scanner.nextLine();
            if (!nifStr.matches("\\d+")) {
                System.out.println("Entrada inválida! O NIF deve conter apenas números.");
                continue;
            }

            nifCliente = Integer.parseInt(nifStr);

            // Verifica se o cliente existe
            for (Cliente cliente : listaClientes) {
                if (cliente.getNif() == nifCliente) {
                    clienteSelecionado = cliente;
                    break;
                }
            }

            if (clienteSelecionado == null) {
                System.out.println("Cliente com NIF " + nifCliente + " não encontrado. Tente novamente.");
            } else {
                break;
            }
        } while (true);

        // Validação da data
        Data dataFatura;
        do {
            System.out.println("\nInsira a data da fatura (formato: AAAA-MM-DD): ");
            String dataString = scanner.nextLine();
            if (!dataString.matches("\\d{4}-\\d{2}-\\d{2}")) {
                System.out.println("Formato de data inválido! Certifique-se de usar o formato AAAA-MM-DD.");
                continue;
            }

            try {
                String[] partesData = dataString.split("-");
                int ano = Integer.parseInt(partesData[0]);
                int mes = Integer.parseInt(partesData[1]);
                int dia = Integer.parseInt(partesData[2]);
                dataFatura = new Data(ano, mes, dia);

                if (!dataFatura.isDataValida()) {
                    System.out.println("Data inválida! Certifique-se de que a data é válida.");
                    continue;
                }
                break;
            } catch (NumberFormatException e) {
                System.out.println("Erro ao interpretar a data. Certifique-se de que contém apenas números no formato correto.");
            }
        } while (true);

        // Adicionar produtos à fatura
        ArrayList<Produto> listaProdutosSelecionados = new ArrayList<>();
        boolean adicionarMais = true;

        while (adicionarMais) {
            System.out.println("\nQue tipo de produto deseja adicionar?");
            System.out.println("1. Produto Alimentar");
            System.out.println("2. Produto de Farmácia");
            System.out.print("Escolha (1 ou 2): ");
            String escolha = scanner.nextLine();

            Produto produto = null;

            if (escolha.equals("1")) {
                // Criar Produto Alimentar
                System.out.print("Insira o código do produto (apenas números): ");
                String codigoStr = scanner.nextLine();
                while (!codigoStr.matches("\\d+")) {
                    System.out.print("Código inválido! Insira apenas números: ");
                    codigoStr = scanner.nextLine();
                }
                int codigo = Integer.parseInt(codigoStr);

                System.out.print("Insira o nome do produto (somente letras e espaços): ");
                String nome = scanner.nextLine();
                while (!nome.matches("[a-zA-ZÀ-ÿ ]+")) {
                    System.out.print("Nome inválido! Use apenas letras e espaços: ");
                    nome = scanner.nextLine();
                }

                System.out.print("Insira a descrição do produto (somente letras e espaços): ");
                String descricao = scanner.nextLine();
                while (!descricao.matches("[a-zA-ZÀ-ÿ ]+")) {
                    System.out.print("Descrição inválida! Use apenas letras e espaços: ");
                    descricao = scanner.nextLine();
                }

                System.out.print("Insira a quantidade disponível (apenas números): ");
                String quantidadeStr = scanner.nextLine();
                while (!quantidadeStr.matches("\\d+")) {
                    System.out.print("Quantidade inválida! Insira apenas números: ");
                    quantidadeStr = scanner.nextLine();
                }
                int quantidade = Integer.parseInt(quantidadeStr);

                System.out.print("Insira o valor do produto (formato X.X): ");
                String valorStr = scanner.nextLine();
                while (!valorStr.matches("\\d+\\.\\d+")) {
                    System.out.print("Valor inválido! Insira no formato X.X: ");
                    valorStr = scanner.nextLine();
                }
                float valor = Float.parseFloat(valorStr);

                System.out.print("O produto é biológico? (true/false): ");
                String biologicoStr = scanner.nextLine();
                while (!biologicoStr.matches("true|false")) {
                    System.out.print("Entrada inválida! Digite 'true' ou 'false': ");
                    biologicoStr = scanner.nextLine();
                }
                boolean biologico = Boolean.parseBoolean(biologicoStr);

                System.out.print("Insira as certificações (separadas por vírgulas, somente letras e espaços): ");
                String certificacoesInput = scanner.nextLine();
                while (!certificacoesInput.matches("([a-zA-ZÀ-ÿ ]+,?)+")) {
                    System.out.print("Certificações inválidas! Use letras, espaços e separadas por vírgulas: ");
                    certificacoesInput = scanner.nextLine();
                }
                ArrayList<String> certificacoes = new ArrayList<>(Arrays.asList(certificacoesInput.split(",\\s*")));

                System.out.print("Insira a categoria do produto (somente letras): ");
                String categoria = scanner.nextLine();
                while (!categoria.matches("[a-zA-ZÀ-ÿ ]+")) {
                    System.out.print("Categoria inválida! Use apenas letras: ");
                    categoria = scanner.nextLine();
                }

                produto = new Alimentar(codigo, nome, descricao, quantidade, valor, biologico, certificacoes, categoria);

            } else if (escolha.equals("2")) {
                // Criar Produto de Farmácia
                System.out.print("Insira o código do produto (apenas números): ");
                String codigoStr = scanner.nextLine();
                while (!codigoStr.matches("\\d+")) {
                    System.out.print("Código inválido! Insira apenas números: ");
                    codigoStr = scanner.nextLine();
                }
                int codigo = Integer.parseInt(codigoStr);

                System.out.print("Insira o nome do produto (somente letras e espaços): ");
                String nome = scanner.nextLine();
                while (!nome.matches("[a-zA-ZÀ-ÿ ]+")) {
                    System.out.print("Nome inválido! Use apenas letras e espaços: ");
                    nome = scanner.nextLine();
                }

                System.out.print("Insira a descrição do produto (somente letras e espaços): ");
                String descricao = scanner.nextLine();
                while (!descricao.matches("[a-zA-ZÀ-ÿ ]+")) {
                    System.out.print("Descrição inválida! Use apenas letras e espaços: ");
                    descricao = scanner.nextLine();
                }

                System.out.print("Insira a quantidade disponível (apenas números): ");
                String quantidadeStr = scanner.nextLine();
                while (!quantidadeStr.matches("\\d+")) {
                    System.out.print("Quantidade inválida! Insira apenas números: ");
                    quantidadeStr = scanner.nextLine();
                }
                int quantidade = Integer.parseInt(quantidadeStr);

                System.out.print("Insira o valor do produto (formato X.X): ");
                String valorStr = scanner.nextLine();
                while (!valorStr.matches("\\d+\\.\\d+")) {
                    System.out.print("Valor inválido! Insira no formato X.X: ");
                    valorStr = scanner.nextLine();
                }
                float valor = Float.parseFloat(valorStr);

                System.out.print("É necessária prescrição médica? (true/false): ");
                String prescricaoStr = scanner.nextLine();
                while (!prescricaoStr.matches("true|false")) {
                    System.out.print("Entrada inválida! Digite 'true' ou 'false': ");
                    prescricaoStr = scanner.nextLine();
                }
                boolean prescricao = Boolean.parseBoolean(prescricaoStr);

                System.out.print("Insira o nome do médico (somente letras e espaços, se aplicável): ");
                String medico = scanner.nextLine();
                while (!medico.matches("[a-zA-ZÀ-ÿ ]*")) {
                    System.out.print("Nome inválido! Use apenas letras e espaços (ou deixe em branco): ");
                    medico = scanner.nextLine();
                }

                System.out.print("Insira a categoria do produto (somente letras): ");
                String categoria = scanner.nextLine();
                while (!categoria.matches("[a-zA-ZÀ-ÿ ]+")) {
                    System.out.print("Categoria inválida! Use apenas letras: ");
                    categoria = scanner.nextLine();
                }

                produto = new Farmacia(codigo, nome, descricao, quantidade, valor , prescricao, medico, categoria);

            } else {
                System.out.println("Escolha inválida. Tente novamente.");
                continue;
            }

            listaProdutosSelecionados.add(produto);

            System.out.print("Deseja adicionar mais produtos? (s/n): ");
            String resposta = scanner.nextLine();
            if (!resposta.equalsIgnoreCase("s")) {
                adicionarMais = false;
            }
        }

        if (listaProdutosSelecionados.isEmpty()) {
            System.out.println("Nenhum produto foi adicionado. Fatura cancelada.");
            return;
        }

        // Criação da fatura
        Fatura novaFatura = new Fatura(numeroFatura, clienteSelecionado, dataFatura, listaProdutosSelecionados);
        listaFaturas.add(novaFatura);

        System.out.println("Fatura criada com sucesso!");
        System.out.println(novaFatura);
    }
    public void carregarFaturas(String nomeFicheiro) {
        ArrayList<Cliente> listaClientes = this.getListaClientes();
        ArrayList<Fatura> listaFaturas = this.getListaFaturas();

        try (BufferedReader reader = new BufferedReader(new FileReader(nomeFicheiro))) {
            String linha;
            setImportouFaturas(true);
            while ((linha = reader.readLine()) != null) {

                // Dividir a linha entre os campos gerais da fatura e a lista de produtos
                String[] dados = linha.split(",", 4); // Garante que apenas a 4ª parte contém produtos
                int numeroFatura = Integer.parseInt(dados[0]);
                int nifCliente = Integer.parseInt(dados[1]);

                // Processar a data
                String[] dataPartes = dados[2].split("-");
                int ano = Integer.parseInt(dataPartes[0]);
                int mes = Integer.parseInt(dataPartes[1]);
                int dia = Integer.parseInt(dataPartes[2]);
                Data dataFatura = new Data(ano, mes, dia);

                // Procurar cliente

                Cliente cliente = procurarClientePeloNif(nifCliente, listaClientes);


                // Processar produtos da fatura
                ArrayList<Produto> listaProdutosFatura = new ArrayList<>();
                if (dados.length == 4) { // Verifica se existe a lista de produtos
                    String[] produtos = dados[3].split("-"); // Separar produtos por ";"
                    for (String produtoInfo : produtos) {
                        try {
                            // Separar os atributos do produto
                            String[] atributos = produtoInfo.split(",");

                            // Extrair informações do produto
                            int codigo = Integer.parseInt(atributos[0]);
                            String tipo = atributos[1];
                            String nome = atributos[2];
                            String descricao = atributos[3];
                            int quantidade = Integer.parseInt(atributos[4]);
                            float valor = Float.parseFloat(atributos[5]);

                            // Verificar o tipo do produto e criar instâncias específicas
                            if (tipo.equals("A")) { // Produto alimentar
                                boolean biologico = Boolean.parseBoolean(atributos[6]);
                                ArrayList<String> certificacoes = new ArrayList<>(Arrays.asList(atributos[7].split("\\|")));
                                String categoria = atributos[8];

                                // Criar instância de produto alimentar
                                Alimentar produto = new Alimentar(codigo, nome, descricao, quantidade, valor, biologico, certificacoes, categoria);
                                listaProdutosFatura.add(produto);

                            } else if (tipo.equals("F")) { // Produto de farmácia
                                boolean prescricao = Boolean.parseBoolean(atributos[6]);

                                if (prescricao && atributos.length > 7) {
                                    // Se há prescrição, o médico é obrigatório
                                    String medico = atributos[7];
                                    Farmacia produto = new Farmacia(codigo, nome, descricao, quantidade, valor, prescricao, medico, null);
                                    listaProdutosFatura.add(produto);
                                } else if (!prescricao && atributos.length > 7) {
                                    // Se não há prescrição, a categoria é obrigatória
                                    String categoria = atributos[7];
                                    Farmacia produto = new Farmacia(codigo, nome, descricao, quantidade, valor, categoria, prescricao);
                                    listaProdutosFatura.add(produto);
                                } else {
                                    System.out.println("Erro nos dados do produto: campos insuficientes -> " + produtoInfo);
                                }
                            }
                        } catch (Exception ex) {
                            System.out.println("Erro ao processar produto: " + produtoInfo + " -> " + ex.getMessage());
                        }
                    }
                }

                // Adicionar a fatura à lista
                if (cliente != null) {
                    Fatura fatura = new Fatura(numeroFatura, cliente, dataFatura, listaProdutosFatura);
                    listaFaturas.add(fatura);
                } else {
                    System.out.println("Cliente com NIF " + nifCliente + " não encontrado.");
                }
            }
            System.out.println("Faturas adicionadas com sucesso.");

        } catch (IOException e) {
            System.out.println("Erro ao ler o ficheiro: " + e.getMessage());
        } catch (Exception e) {
            System.out.println("Erro ao processar dados: " + e.getMessage());
        }
    }

    public Cliente procurarClientePeloNif(int nif, ArrayList<Cliente> listaClientes) {
        for (Cliente cliente : listaClientes) {
            if (cliente.getNif() == nif) {
                return cliente;
            }
        }
        return null;
    }


    public void editarFatura(){

        Scanner scanner = new Scanner(System.in);
        ArrayList<Fatura> listaFaturas = this.getListaFaturas();

        System.out.println("Insira o número da fatura que deseja editar:");
        int numeroFatura = scanner.nextInt();

        Fatura faturaEncontrada = null;
        for (Fatura f : listaFaturas) {
            if (f.getNumeroFatura() == numeroFatura) {
                f.editarFatura();
                faturaEncontrada = f;
                break;
            }
        }

        if (faturaEncontrada == null) {
            System.out.println("Fatura com o número " + numeroFatura + " não encontrada.");
        }

    }

    public ArrayList<Cliente> getListaClientes() {
        return listaClientes;
    }

    public void setListaClientes(ArrayList<Cliente> listaClientes) {
        this.listaClientes = listaClientes;
    }

    public ArrayList<Fatura> getListaFaturas() {
        return listaFaturas;
    }

    public boolean isImportouFaturas() {
        return importouFaturas;
    }

    public void setImportouFaturas(boolean importouFaturas) {
        this.importouFaturas = importouFaturas;
    }

    public void setListaFaturas(ArrayList<Fatura> listaFaturas) {
        this.listaFaturas = listaFaturas;
    }
}