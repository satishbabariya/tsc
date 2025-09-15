
// Vue-like Composition API Test
import { ref, computed, watch, onMounted, onUnmounted } from 'vue';

// Generic composable
function useGenericComposable<T>(initialValue: T) {
    const state = ref<T>(initialValue);
    const loading = ref(false);
    const error = ref<string | null>(null);
    
    const setState = (value: T) => {
        state.value = value;
    };
    
    const setLoading = (value: boolean) => {
        loading.value = value;
    };
    
    const setError = (value: string | null) => {
        error.value = value;
    };
    
    const reset = () => {
        state.value = initialValue;
        loading.value = false;
        error.value = null;
    };
    
    return {
        state: readonly(state),
        loading: readonly(loading),
        error: readonly(error),
        setState,
        setLoading,
        setError,
        reset
    };
}

// Generic async composable
function useAsyncComposable<T>(
    asyncFn: () => Promise<T>,
    initialValue: T
) {
    const { state, loading, error, setState, setLoading, setError } = 
        useGenericComposable(initialValue);
    
    const execute = async () => {
        setLoading(true);
        setError(null);
        
        try {
            const result = await asyncFn();
            setState(result);
        } catch (err) {
            setError(err instanceof Error ? err.message : 'Unknown error');
        } finally {
            setLoading(false);
        }
    };
    
    onMounted(() => {
        execute();
    });
    
    return {
        state,
        loading,
        error,
        execute
    };
}

// Generic form composable
function useFormComposable<T extends Record<string, any>>(
    initialValues: T,
    validationRules?: Partial<Record<keyof T, (value: any) => string | null>>
) {
    const formData = ref<T>({ ...initialValues });
    const errors = ref<Partial<Record<keyof T, string>>>({});
    const touched = ref<Partial<Record<keyof T, boolean>>>({});
    
    const setFieldValue = <K extends keyof T>(field: K, value: T[K]) => {
        formData.value[field] = value;
        touched.value[field] = true;
        
        if (validationRules?.[field]) {
            const error = validationRules[field]!(value);
            errors.value[field] = error || undefined;
        }
    };
    
    const validateField = <K extends keyof T>(field: K) => {
        if (validationRules?.[field]) {
            const error = validationRules[field]!(formData.value[field]);
            errors.value[field] = error || undefined;
        }
    };
    
    const validateForm = () => {
        const newErrors: Partial<Record<keyof T, string>> = {};
        
        if (validationRules) {
            for (const field in validationRules) {
                const error = validationRules[field]!(formData.value[field]);
                if (error) {
                    newErrors[field] = error;
                }
            }
        }
        
        errors.value = newErrors;
        return Object.keys(newErrors).length === 0;
    };
    
    const resetForm = () => {
        formData.value = { ...initialValues };
        errors.value = {};
        touched.value = {};
    };
    
    const isFieldTouched = <K extends keyof T>(field: K) => {
        return touched.value[field] || false;
    };
    
    const hasFieldError = <K extends keyof T>(field: K) => {
        return !!errors.value[field];
    };
    
    const getFieldError = <K extends keyof T>(field: K) => {
        return errors.value[field];
    };
    
    return {
        formData: readonly(formData),
        errors: readonly(errors),
        touched: readonly(touched),
        setFieldValue,
        validateField,
        validateForm,
        resetForm,
        isFieldTouched,
        hasFieldError,
        getFieldError
    };
}

// Usage example
interface UserForm {
    name: string;
    email: string;
    age: number;
}

const userForm = useFormComposable<UserForm>(
    { name: '', email: '', age: 0 },
    {
        name: (value) => value.length < 2 ? 'Name must be at least 2 characters' : null,
        email: (value) => !value.includes('@') ? 'Invalid email format' : null,
        age: (value) => value < 18 ? 'Must be at least 18 years old' : null
    }
);

console.log("Vue-like composition test completed");
