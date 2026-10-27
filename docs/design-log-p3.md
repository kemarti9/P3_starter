# Project 3 Design Log
**Author:** <Kennedy Martin> · **P2 code used:** <instructor reference>
**Tools used:** <AI assistant for debugging, troubleshooting, and polishing design log>


## 1. Hash table

I used separate chaining where each bucket holds a short list. "Collisions" share a list and "find" searches that list. In seperate chaining, a search for a missing name always ends at the end of a short list.

My hash is FNV-1a, which turns a string of text into a number by processing each character one at a time. The algorithm starts wih a fixxed number and then combines the numeric value of the next character with the current hash. The result is then multiplied by the next prime number. 

I start with 16 buckets and resize at load factor 0.75 (tools divided by buckets). Before each add, I check if it would push the factor over 0.75. If so, I double the buckets, re-insert the old tools, and insert the new tools.

With 10,000 tools, tool_0 to tool_9999, I got bucket_count 16384, load_factor 0.610352, max_probe_length 6. The weak hash s[0] + s.size() would have a longest chain of 9,000 at the same count. The longest list is 6.

Add is amortized O(1). Most adds are hash and add to one short list. A resize moves every tool, but this only occurs after multiple cheap adds, and doubling makes resizes rarer as the table grows, so the cost per add stays about constant.

## 2. Pointer stability

A Tool* from find() survives a resize because my buckets store std::unique_ptr<Tool>. Unique_ptr is the pointer to the tool. Resize moves unique_ptr. The Tool never moves. Find returns that address with get().

## 3. Calculator trace

2 → values [2]. * → ops [*]. ( → ops [*, (]. 3 → values [2, 3]. + → ops [*, (, +], because the top is (, not an operator. 4 → vales [2, 3, 4]. ) → apply +: 3 + 4 = 7, so values is [2, 7], then pop (, so ops is [*]. - → the top of ops is *, which has higher precedence, so apply * first: 2 * 7 = 14, so values is [14] and ops is empty, then push -, so ops is [-]. 5 → values [14, 5]. End → apply -: 14 - 5 = 9, so values is [9]. That is the answer.

## 4. Errors as values

The tool should fail if a file does not exist, a path may is forbidden, or an expression is trying to divide by zero. Return ToolResult::failure with a message, resolve returns false with a reason, and calc_evaluate returns false with a message. The error travels back through the normal code. Throwing would be wrong for these cases because it would not give the model nothing useful to read.

## 5. What I would design differently

I would start with 64 buckets instead of 16. 16 buckets means many resizes on the way to 10,000 tools. Sixty-four is still small in memory and would cut the number of resizes. 
