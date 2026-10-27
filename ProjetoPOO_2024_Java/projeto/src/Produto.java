import java.io.*;

/**
 * Classe abstrata que representa um Produto.
 * Essa classe serve como base para diferentes tipos de produtos, fornecendo atributos
 * e métodos comuns que podem ser especializados nas subclasses.
 */
public abstract class Produto implements Serializable {

    /**
     * Código identificador do produto.
     */
    protected int codigo;

    /**
     * Nome do produto.
     */
    protected String nome;

    /**
     * Descrição do produto.
     */
    protected String descricao;

    /**
     * Quantidade do produto.
     */
    protected int quantidade;

    /**
     * Valor do produto.
     */
    protected float valor;

    /**
     * Construtor completo para inicializar um Produto com todos os atributos.
     *
     * @param codigo     o código  do produto.
     * @param nome       o nome do produto.
     * @param descricao  a descrição do produto.
     * @param quantidade a quantidade  do produto.
     * @param valor      o valor produto.
     */
    public Produto(int codigo, String nome, String descricao, int quantidade, float valor) {
        this.codigo = codigo;
        this.nome = nome;
        this.descricao = descricao;
        this.quantidade = quantidade;
        this.valor = valor;
    }

    /**
     * Construtor padrão que inicializa um Produto vazio.
     */
    public Produto() {
    }

    /**
     * Obtém o código do produto.
     *
     * @return o código do produto.
     */
    public int getCodigo() {
        return codigo;
    }

    /**
     * Define o código do produto.
     *
     * @param codigo o novo código do produto.
     */
    public void setCodigo(int codigo) {
        this.codigo = codigo;
    }

    /**
     * Obtém o nome do produto.
     *
     * @return o nome do produto.
     */
    public String getNome() {
        return nome;
    }

    /**
     * Define o nome do produto.
     *
     * @param nome o novo nome do produto.
     */
    public void setNome(String nome) {
        this.nome = nome;
    }

    /**
     * Obtém a descrição do produto.
     *
     * @return a descrição do produto.
     */
    public String getDescricao() {
        return descricao;
    }

    /**
     * Define a descrição do produto.
     *
     * @param descricao a nova descrição do produto.
     */
    public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

    /**
     * Obtém a quantidade do produto.
     *
     * @return a quantidade do produto.
     */
    public int getQuantidade() {
        return quantidade;
    }

    /**
     * Define a quantidade  do produto.
     *
     * @param quantidade a nova quantidade do produto.
     */
    public void setQuantidade(int quantidade) {
        this.quantidade = quantidade;
    }

    /**
     * Obtém o valor do produto.
     *
     * @return o valor  do produto.
     */
    public float getValor() {
        return valor;
    }

    /**
     * Define o valor do produto.
     *
     * @param valor o novo valor do produto.
     */
    public void setValor(float valor) {
        this.valor = valor;
    }

    /**
     * Método abstrato que calcula a taxa associada ao produto, dependendo do tipo de cliente.
     * Deve ser implementado pelas subclasses.
     *
     * @param c o cliente para o qual a taxa será calculada.
     * @return o valor da taxa aplicada ao produto.
     */
    public abstract double taxa(Cliente c);

    /**
     * Método abstrato para exportar as informações do produto para um arquivo.
     * Deve ser implementado pelas subclasses.
     */
    public abstract String exportar();

    /**
     * Retorna uma representação textual do produto, com seus principais atributos.
     *
     * @return uma string contendo os atributos do produto.
     */
    @Override
    public String toString() {
        return "Produto{" +
                "codigo=" + codigo +
                ", nome='" + nome + '\'' +
                ", descricao='" + descricao + '\'' +
                ", quantidade=" + quantidade +
                ", valor=" + valor +
                '}';
    }
}