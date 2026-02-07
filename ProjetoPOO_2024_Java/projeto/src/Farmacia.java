import java.io.*;

/**
 * A classe {@code Farmacia} representa um produto farmacêutico. É uma especialização da classe {@code Produto},
 * contendo informações adicionais sobre prescrição médica, médico responsável e categoria do produto.
 * A classe também implementa a interface {@code Serializable} para permitir sua serialização.
 */
public class Farmacia extends Produto implements Serializable {

    private boolean prescricao;
    private String medico;
    private String categoria;

    /**
     * Construtor completo para criar uma instância de {@code Farmacia} com todos os atributos.
     *
     * @param codigo     o código do produto.
     * @param nome       o nome do produto.
     * @param descricao  a descrição do produto.
     * @param quantidade a quantidade disponível do produto.
     * @param valor      o valor unitário do produto.
     * @param prescricao indica se o produto requer prescrição médica.
     * @param medico     o médico responsável pela prescrição (se aplicável).
     * @param categoria  a categoria do produto (ex.: medicamentos, animais, etc.).
     */
    public Farmacia(int codigo, String nome, String descricao, int quantidade, float valor, boolean prescricao, String medico, String categoria) {
        super(codigo, nome, descricao, quantidade, valor);
        this.prescricao = prescricao;
        this.medico = medico;
        this.categoria = categoria;
    }

    /**
     * Construtor alternativo para criar uma instância de {@code Farmacia} sem especificar o médico responsável.
     *
     * @param codigo     o código do produto.
     * @param nome       o nome do produto.
     * @param descricao  a descrição do produto.
     * @param quantidade a quantidade disponível do produto.
     * @param valor      o valor unitário do produto.
     * @param categoria  a categoria do produto.
     * @param prescricao indica se o produto requer prescrição médica.
     */
    public Farmacia(int codigo, String nome, String descricao, int quantidade, float valor, String categoria, boolean prescricao) {
        super(codigo, nome, descricao, quantidade, valor);
        this.categoria = categoria;
        this.prescricao = prescricao;
    }

    /**
     * Construtor padrão para criar uma instância vazia de {@code Farmacia}.
     */
    public Farmacia() {
    }

    /**
     * Retorna o nome do médico responsável pela prescrição.
     *
     * @return o nome do médico.
     */
    public String getMedico() {
        return medico;
    }

    /**
     * Define o nome do médico responsável pela prescrição.
     *
     * @param medico o nome do médico.
     */
    public void setMedico(String medico) {
        this.medico = medico;
    }

    /**
     * Retorna a categoria do produto.
     *
     * @return a categoria do produto.
     */
    public String getCategoria() {
        return categoria;
    }

    /**
     * Define a categoria do produto.
     *
     * @param categoria a categoria do produto.
     */
    public void setCategoria(String categoria) {
        this.categoria = categoria;
    }

    /**
     * Retorna se o produto requer prescrição médica.
     *
     * @return {@code true} se o produto requer prescrição; {@code false} caso contrário.
     */
    public boolean isPrescricao() {
        return prescricao;
    }

    /**
     * Define se o produto requer prescrição médica.
     *
     * @param prescricao {@code true} se o produto requer prescrição; {@code false} caso contrário.
     */
    public void setPrescricao(boolean prescricao) {
        this.prescricao = prescricao;
    }

    /**
     * Calcula a taxa de IVA (Imposto sobre o Valor Acrescentado) para o produto com base
     * na localização do cliente, se o produto requer prescrição e sua categoria.
     *
     * @param c o cliente que irá adquirir o produto.
     * @return a taxa de IVA aplicável.
     */
    public double taxa(Cliente c) {
        double taxa = 0.0;
        taxa = prescricao ? (c.getLocalizacao().equalsIgnoreCase("Continente") ? 0.06 :
                c.getLocalizacao().equalsIgnoreCase("Madeira") ? 0.05 : 0.04) : 0.23;
        if (!prescricao && categoria.equalsIgnoreCase("animais")) {
            taxa -= 0.01;
        }
        return taxa;
    }

    /**
     * Exporta as informações do produto no formato CSV.
     * O formato varia dependendo de o produto exigir prescrição ou não.
     *
     * @return uma string formatada com as informações do produto no formato CSV.
     */
    public String exportar() {
        // Formatar o valor com duas casas decimais e substituir a vírgula por ponto
        String valorFormatado = String.format("%.2f", valor).replace(',', '.');

        // Verificar se o produto necessita de prescrição
        if (prescricao) {
            return String.format("%d,F,%s,%s,%d,%s,%b,%s",
                    codigo,
                    nome,
                    descricao,
                    quantidade,
                    valorFormatado,
                    prescricao,
                    medico
            );
        } else {
            return String.format("%d,F,%s,%s,%d,%s,%b,%s",
                    codigo,
                    nome,
                    descricao,
                    quantidade,
                    valorFormatado,
                    prescricao,
                    categoria
            );
        }
    }

    /**
     * Retorna uma representação textual da instância {@code Farmacia},
     * incluindo os atributos da superclasse e os específicos da classe.
     *
     * @return uma string representando a instância.
     */
    @Override
    public String toString() {
        return super.toString() +
                "Farmacia{" +
                "prescricao=" + prescricao +
                ", medico='" + medico + '\'' +
                ", categoria='" + categoria + '\'' +
                '}';
    }
}