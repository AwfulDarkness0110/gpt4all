#ifndef LLMODEL_C_H
#define LLMODEL_C_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opaque pointer to the underlying model.
 */
typedef void *llmodel_model;

/**
 * llmodel_prompt_context structure for holding the prompt context.
 */
typedef struct {
    float *logits;          // logits of current context
    int32_t *tokens;        // current tokens in the context window
    int32_t n_past;         // number of tokens in past conversation
    int32_t n_ctx;          // number of tokens possible in context window
    int32_t n_predict;      // number of tokens to predict
    int32_t top_k;          // top k logits to sample from
    float top_p;            // nucleus sampling probability threshold
    float temp;             // temperature to adjust model's output distribution
    int32_t n_batch;        // number of predictions to generate in parallel
    float repeat_penalty;   // penalty factor for repeated tokens
    int32_t repeat_last_n;  // last n tokens to penalize
    float context_erase;    // percent of context to erase if we exceed the context window
} llmodel_prompt_context;

/**
 * Callback type for response.
 * @param token_id The token id of the response.
 * @param response The response string.
 * @return a bool indicating whether the model should keep generating.
 */
typedef bool (*llmodel_response_callback)(int32_t token_id, const char *response);

/**
 * Callback type for recalculation of context.
 * @param whether the model is recalculating the context.
 * @return a bool indicating whether the model should keep generating.
 */
typedef bool (*llmodel_recalculate_callback)(bool is_recalculating);

/**
 * Create a GPTJ instance.
 * @return A pointer to the GPTJ instance.
 */
llmodel_model llmodel_gptj_create();

/**
 * Destroy a GPTJ instance.
 * @param gptj A pointer to the GPTJ instance.
 */
void llmodel_gptj_destroy(llmodel_model gptj);

/**
 * Create a LLAMA instance.
 * @return A pointer to the LLAMA instance.
 */
llmodel_model llmodel_llama_create();

/**
 * Destroy a LLAMA instance.
 * @param llama A pointer to the LLAMA instance.
 */
void llmodel_llama_destroy(llmodel_model llama);

/**
 * Load a model from a file.
 * @param model A pointer to the llmodel_model instance.
 * @param model_path A string representing the path to the model file.
 * @return true if the model was loaded successfully, false otherwise.
 */
bool llmodel_loadModel(llmodel_model model, const char *model_path);

/**
 * Check if a model is loaded.
 * @param model A pointer to the llmodel_model instance.
 * @return true if the model is loaded, false otherwise.
 */
bool llmodel_isModelLoaded(llmodel_model model);

/**
 * Generate a response using the model.
 * @param model A pointer to the llmodel_model instance.
 * @param prompt A string representing the input prompt.
 * @param response A callback function for handling the generated response.
 * @param recalculate A callback function for handling recalculation requests.
 * @param ctx A pointer to the llmodel_prompt_context structure.
 */
void llmodel_prompt(llmodel_model model, const char *prompt,
                    llmodel_response_callback response,
                    llmodel_recalculate_callback recalculate,
                    llmodel_prompt_context *ctx);

/**
 * Set the number of threads to be used by the model.
 * @param model A pointer to the llmodel_model instance.
 * @param n_threads The number of threads to be used.
 */
void llmodel_setThreadCount(llmodel_model model, int32_t n_threads);

/**
 * Get the number of threads currently being used by the model.
 * @param model A pointer to the llmodel_model instance.
 * @return The number of threads currently being used.
 */
int32_t llmodel_threadCount(llmodel_model model);

#ifdef __cplusplus
}
#endif

#endif // LLMODEL_C_H
