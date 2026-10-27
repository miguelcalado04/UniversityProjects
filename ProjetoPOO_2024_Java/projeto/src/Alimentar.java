import java.util.ArrayList;
import java.io.*;

/**
 * Representa um produto alimentar, que herda da classe {@link Produto}.
 * Este tipo de produto pode ser classificado como biológico, conter certificações,
 * e pertencer a uma categoria específica.
 *
 * <p>Inclui métodos para calcular a taxa de imposto aplicada com base na localização
 * do cliente, nas certificações e em características do produto.</p>
 *
 * @author [Seu Nome]
 * @version 1.0
 */
public class Alimentar extends Produto implements Serializable {

    /**
     * Indica se o produto é biológico.
     */
    private boolean biologico;

    /**
     * Lista de certificações atribuídas ao produto.
     */
    private ArrayList<String> certificacoes;

    /**
     * Categoria do produto alimentar (ex: congelados, enlatados, etc.).
     */
    private String categoria;

    /**
     * Construtor completo da classe Alimentar.
     *
     * @param codigo         Código identificador do produto.
     * @param nome           Nome do produto.
     * @param descricao      Descrição do produto.
     * @param quantidade     Quantidade disponível em estoque.
     * @param valor          Valor unitário do produto.
     * @param biologico      Indica se o produto é biológico.
     * @param certificacoes  Lista de certificações atribuídas ao produto.
     * @param categoria      Categoria do produto alimentar.
     */
    public Alimentar(int codigo, String nome, String descricao, int quantidade, float valor, boolean biologico, ArrayList<String> certificacoes, String categoria) {
        super(codigo, nome, descricao, quantidade, valor);
        this.biologico = biologico;
        this.certificacoes = certificacoes;
        this.categoria = categoria;
    }

    /**
     * Construtor vazio da classe Alimentar.
     */
    public Alimentar() {
    }

    /**
     * Verifica se o produto é biológico.
     *
     * @return {@code true} se o produto for biológico, caso contrário {@code false}.
     */
    public boolean isBiologico() {
        return biologico;
    }

    /**
     * Define se o produto é biológico.
     *
     * @param biologico {@code true} para definir como biológico, caso contrário {@code false}.
     */
    public void setBiologico(boolean biologico) {
        this.biologico = biologico;
    }

    /**
     * Retorna a lista de certificações do produto.
     *
     * @return Lista de certificações do produto.
     */
    public ArrayList<String> getCertificacoes() {
        return certificacoes;
    }

    /**
     * Define as certificações do produto.
     *
     * @param certificacoes Lista de certificações.
     */
    public void setCertificacoes(ArrayList<String> certificacoes) {
        this.certificacoes = certificacoes;
    }

    /**
     * Retorna a categoria do produto.
     *
     * @return Categoria do produto.
     */
    public String getCategoria() {
        return categoria;
    }

    /**
     * Define a categoria do produto.
     *
     * @param categoria Categoria a ser atribuída ao produto.
     */
    public void setCategoria(String categoria) {
        this.categoria = categoria;
    }

    /**
     * Calcula a taxa de imposto a ser aplicada com base na localização do cliente,
     * nas características do produto e nas certificações atribuídas.
     *
     * <ul>
     *     <li>Se o produto for biológico, há uma redução de 10% na taxa.</li>
     *     <li>Certificações e categoria do produto (ex.: congelados, vinho) podem alterar a taxa.</li>
     * </ul>
     *
     * @param c Cliente cuja localização é usada para determinar a taxa.
     * @return Taxa de imposto calculada.
     */
    public double taxa(Cliente c) {
        double taxa = 0;

        if (!this.certificacoes.isEmpty()) {
            taxa = c.getLocalizacao().equalsIgnoreCase("Continente") ? 0.06 :
                    c.getLocalizacao().equalsIgnoreCase("Madeira") ? 0.05 : 0.04;

            if (this.certificacoes.size() == 4) {
                taxa -= 0.01; // Reduz 1% se houver exatamente 4 certificações
            }
        } else if (this.categoria.equalsIgnoreCase("congelados") ||
                this.categoria.equalsIgnoreCase("enlatados") ||
                this.categoria.equalsIgnoreCase("vinho")) {
            taxa = c.getLocalizacao().equalsIgnoreCase("Continente") ? 0.13 :
                    c.getLocalizacao().equalsIgnoreCase("Madeira") ? 0.12 : 0.09;

            if (this.categoria.equalsIgnoreCase("vinho")) {
                taxa += 0.01; // Adiciona 1% se for vinho
            }
        } else {
            taxa = c.getLocalizacao().equalsIgnoreCase("Continente") ? 0.23 :
                    c.getLocalizacao().equalsIgnoreCase("Madeira") ? 0.22 : 0.16;
        }

        if (this.biologico) {
            taxa -= taxa * 0.10; // Reduz 10% se for biológico
        }

        return taxa;
    }

    /**
     * Exporta os dados do produto no formato de string.
     *
     * @return String formatada contendo os dados do produto.
     */
    @Override
    public String exportar() {
        // Preparar as certificações no formato correto
        String certificacoesString = certificacoes != null ? String.join("|", certificacoes) : "";

        // Formatar o valor com duas casas decimais e substituindo a vírgula por ponto
        String valorFormatado = String.format("%.2f", valor).replace(',', '.');

        // Retornar os dados do produto no formato de string
        return String.format("%d,A,%s,%s,%d,%s,%b,%s,%s",
                codigo,
                nome,
                descricao,
                quantidade,
                valorFormatado,
                biologico,
                certificacoesString,
                categoria);
    }

    /**
     * Retorna uma representação textual do produto alimentar.
     *
     * @return String representando os atributos do produto alimentar.
     */
    @Override
    public String toString() {
        return super.toString() +
                "Alimentar{" +
                "biologico=" + biologico +
                ", certificacoes=" + certificacoes +
                ", categoria='" + categoria + '\'' +
                '}';
    }
}
