import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.Scanner;
import java.util.Arrays;
import java.io.*;

/**
 * A classe Fatura representa uma fatura contendo informações sobre o cliente, data,
 * e a lista de produtos adquiridos.
 * É responsável por funcionalidades como imprimir faturas, visualizar detalhes,
 * e editar a lista de produtos.
 */
public class Fatura implements Serializable {

    private int numeroFatura;
    private Cliente cliente;
    private Data data;
    private ArrayList<Produto> listaProdutos;

    /**
     * Construtor completo para inicializar uma fatura com todos os seus atributos.
     *
     * @param numeroFatura o número único da fatura.
     * @param cliente o cliente associado à fatura.
     * @param data a data de emissão da fatura.
     * @param listaProdutos a lista de produtos incluídos na fatura.
     */
    public Fatura(int numeroFatura, Cliente cliente, Data data, ArrayList<Produto> listaProdutos) {
        this.numeroFatura = numeroFatura;
        this.cliente = cliente;
        this.data = data;
        this.listaProdutos = listaProdutos;
    }

    /**
     * Construtor vazio para criar uma fatura sem atributos definidos.
     */
    public Fatura() {
    }

    /**
     * Imprime um resumo das faturas, incluindo o número, cliente, localização,
     * quantidade de produtos, preço sem IVA e com IVA.
     */
    public void imprimirFaturas() {
        double precoSemIva = 0;
        double precoComIva = 0;

        ArrayList<Produto> produtosFatura = this.getListaProdutos();
        for (Produto p : produtosFatura) {
            double valorProduto = p.getValor();
            double taxa = p.taxa(this.getCliente());
            double precoSemIvaPro = valorProduto * p.getQuantidade();
            precoSemIva += precoSemIvaPro;
            precoComIva += precoSemIvaPro + (precoSemIvaPro * taxa);
        }

        System.out.printf("%-9d | %-8s | %-11s | %-10d | %-12.2f | %.2f%n",
                this.getNumeroFatura(),
                this.getCliente().getNome(),
                this.getCliente().getLocalizacao(),
                produtosFatura.size(),
                precoSemIva,
                precoComIva
        );

        System.out.println("------------------------------------------------------------");
    }

    /**
     * Exibe os detalhes completos da fatura, incluindo cliente, data, produtos,
     * e totais com e sem IVA.
     */
    public void visualizarFatura() {
        System.out.println("\n=== Fatura Detalhada ===");
        System.out.println("Número da Fatura: " + this.getNumeroFatura());
        System.out.println("Cliente: " + this.getCliente());
        System.out.println("Data da Fatura: " + this.getData());
        System.out.println("\n--- Produtos ---");

        double totalSemIva = 0;
        double totalIva = 0;
        double totalComIva = 0;

        for (Produto produto : this.getListaProdutos()) {
            double valorSemIva = produto.getValor();
            double taxaIva = produto.taxa(this.getCliente());
            double valorIva = valorSemIva * taxaIva;
            double valorComIva = valorSemIva + valorIva;

            totalSemIva += valorSemIva;
            totalIva += valorIva;
            totalComIva += valorComIva;

            System.out.println("Produto: " + produto.getNome());
            System.out.println("Descrição: " + produto.getDescricao());
            System.out.println("Preço Sem IVA: " + String.format("%.2f", valorSemIva));
            System.out.println("Taxa de IVA: " + String.format("%.2f", taxaIva * 100) + "%");
            System.out.println("Valor do IVA: " + String.format("%.2f", valorIva));
            System.out.println("Preço com IVA: " + String.format("%.2f", valorComIva));
            System.out.println("-------------------");
        }

        System.out.println("\n--- Totais da Fatura ---");
        System.out.println("Total Sem IVA: " + String.format("%.2f", totalSemIva));
        System.out.println("Total do IVA: " + String.format("%.2f", totalIva));
        System.out.println("Total Com IVA: " + String.format("%.2f", totalComIva));
        System.out.println("=========================");
    }

    /**
     * Permite editar a lista de produtos da fatura, com opções para adicionar,
     * remover e finalizar edição.
     */
    public void editarFatura() {
        if (this == null) {
            System.out.println("Fatura inválida. Operação cancelada.");
            return;
        }

        Scanner scanner = new Scanner(System.in);
        ArrayList<Produto> novaListaProdutos = new ArrayList<>(this.getListaProdutos());
        boolean editarProdutos = true;

        while (editarProdutos) {
            System.out.println("\n=== Editar Produtos na Fatura ===");

            if (novaListaProdutos.isEmpty()) {
                System.out.println("Nenhum produto na fatura atualmente.");
            } else {
                System.out.println("Produtos na fatura:");
                for (int i = 0; i < novaListaProdutos.size(); i++) {
                    System.out.println((i + 1) + ". " + novaListaProdutos.get(i));
                }
            }

            System.out.println("\nOpções:");
            System.out.println("1. Adicionar produto");
            System.out.println("2. Remover produto");
            System.out.println("3. Finalizar edição");
            System.out.print("Escolha uma opção: ");

            int escolha = validarEntradaInteiro(scanner, 1, 3);

            switch (escolha) {
                case 1:
                    // Código para adicionar produto
                    break;
                case 2:
                    // Código para remover produto
                    break;
                case 3:
                    editarProdutos = false;
                    break;
                default:
                    System.out.println("Opção inválida! Escolha entre 1, 2 ou 3.");
            }
        }

        this.setListaProdutos(novaListaProdutos);
        System.out.println("Edição concluída. Produtos atualizados na fatura.");
    }

    /**
     * Procura por um produto na lista de produtos da fatura com base no código.
     *
     * @param codigo o código do produto.
     * @param listaProdutos a lista de produtos onde será feita a busca.
     * @return o produto encontrado ou {@code null} caso não exista.
     */
    public Produto procurarProdutoFatura(int codigo, ArrayList<Produto> listaProdutos) {
        for (Produto p : listaProdutos) {
            if (p.getCodigo() == codigo) {
                return p;
            }
        }
        return null;
    }

    /**
     * Valida a entrada de um número inteiro dentro de um intervalo específico.
     *
     * @param scanner o objeto {@code Scanner} para leitura.
     * @param min o valor mínimo permitido.
     * @param max o valor máximo permitido.
     * @return o número inteiro validado.
     */
    private int validarEntradaInteiro(Scanner scanner, int min, int max) {
        int valor;
        while (true) {
            try {
                valor = scanner.nextInt();
                if (valor >= min && valor <= max) {
                    return valor;
                } else {
                    System.out.println("Entrada inválida! Insira um número entre " + min + " e " + max + ".");
                }
            } catch (InputMismatchException e) {
                System.out.println("Entrada inválida! Insira um número inteiro.");
                scanner.next();
            }
        }
    }

    /**
     * Obtém o número da fatura.
     *
     * @return o número da fatura.
     */
    public int getNumeroFatura() {
        return numeroFatura;
    }

    /**
     * Define o número da fatura.
     *
     * @param numeroFatura o novo número da fatura.
     */
    public void setNumeroFatura(int numeroFatura) {
        this.numeroFatura = numeroFatura;
    }

    /**
     * Obtém o cliente associado à fatura.
     *
     * @return o cliente da fatura.
     */
    public Cliente getCliente() {
        return cliente;
    }

    /**
     * Define o cliente associado à fatura.
     *
     * @param cliente o novo cliente da fatura.
     */
    public void setCliente(Cliente cliente) {
        this.cliente = cliente;
    }

    /**
     * Obtém a data de emissão da fatura.
     *
     * @return a data da fatura.
     */
    public Data getData() {
        return data;
    }

    /**
     * Define a data de emissão da fatura.
     *
     * @param data a nova data da fatura.
     */
    public void setData(Data data) {
        this.data = data;
    }

    /**
     * Obtém a lista de produtos da fatura.
     *
     * @return a lista de produtos.
     */
    public ArrayList<Produto> getListaProdutos() {
        return listaProdutos;
    }

    /**
     * Define a lista de produtos da fatura.
     *
     * @param listaProdutos a nova lista de produtos.
     */
    public void setListaProdutos(ArrayList<Produto> listaProdutos) {
        this.listaProdutos = listaProdutos;
    }
}