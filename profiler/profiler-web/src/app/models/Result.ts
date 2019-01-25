class Result {
    type: string;
    results: [{
        jnz: number,
        jnz_new: number,
        n: number,
        name: string,
        nnz: number
    }];

    constructor(type, results) {
        this.type = type;
        this.results = results;
    }
}