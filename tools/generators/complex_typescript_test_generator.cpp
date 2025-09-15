#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::cout << "=== Complex TypeScript Test Generator ===" << std::endl;
    
    // Generate React-like component test
    std::string reactTest = R"(
// React-like Component System Test
import React, { useState, useEffect, useCallback } from 'react';

// Generic component props
interface ComponentProps<T = any> {
    data: T;
    onUpdate?: (data: T) => void;
    children?: React.ReactNode;
}

// Generic component state
interface ComponentState<T> {
    data: T;
    loading: boolean;
    error: string | null;
}

// Generic hook
function useGenericState<T>(initialValue: T): [T, (value: T) => void] {
    const [state, setState] = useState<T>(initialValue);
    return [state, setState];
}

// Generic component
function GenericComponent<T>(props: ComponentProps<T>): JSX.Element {
    const [state, setState] = useGenericState<ComponentState<T>>({
        data: props.data,
        loading: false,
        error: null
    });
    
    const handleUpdate = useCallback((newData: T) => {
        setState(prev => ({ ...prev, data: newData }));
        props.onUpdate?.(newData);
    }, [props, setState]);
    
    useEffect(() => {
        setState(prev => ({ ...prev, loading: true }));
        // Simulate async operation
        setTimeout(() => {
            setState(prev => ({ ...prev, loading: false }));
        }, 1000);
    }, [setState]);
    
    return (
        <div>
            <h1>Generic Component</h1>
            <p>Data: {JSON.stringify(state.data)}</p>
            <p>Loading: {state.loading ? 'Yes' : 'No'}</p>
            {state.error && <p>Error: {state.error}</p>}
            {props.children}
        </div>
    );
}

// Usage
const UserComponent = GenericComponent<{ id: number; name: string }>;
const ProductComponent = GenericComponent<{ id: number; title: string; price: number }>;

console.log("React-like component test completed");
)";
    
    std::ofstream reactFile("react_component_test.ts");
    if (reactFile.is_open()) {
        reactFile << reactTest;
        reactFile.close();
        std::cout << "Generated: react_component_test.ts" << std::endl;
    }
    
    // Generate Angular-like service test
    std::string angularTest = R"(
// Angular-like Service System Test
import { Injectable, Inject } from '@angular/core';
import { Observable, BehaviorSubject } from 'rxjs';

// Generic service interface
interface ServiceConfig<T> {
    endpoint: string;
    transform?: (data: any) => T;
    cache?: boolean;
}

// Generic HTTP service
@Injectable()
class HttpService<T> {
    private cache = new Map<string, T>();
    
    constructor(@Inject('config') private config: ServiceConfig<T>) {}
    
    get(id: string): Observable<T> {
        if (this.config.cache && this.cache.has(id)) {
            return new BehaviorSubject(this.cache.get(id)!);
        }
        
        return new Observable(observer => {
            fetch(`${this.config.endpoint}/${id}`)
                .then(response => response.json())
                .then(data => {
                    const transformedData = this.config.transform ? 
                        this.config.transform(data) : data;
                    
                    if (this.config.cache) {
                        this.cache.set(id, transformedData);
                    }
                    
                    observer.next(transformedData);
                    observer.complete();
                })
                .catch(error => observer.error(error));
        });
    }
    
    post(data: Partial<T>): Observable<T> {
        return new Observable(observer => {
            fetch(this.config.endpoint, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(data)
            })
            .then(response => response.json())
            .then(result => {
                const transformedData = this.config.transform ? 
                    this.config.transform(result) : result;
                observer.next(transformedData);
                observer.complete();
            })
            .catch(error => observer.error(error));
        });
    }
}

// Generic repository pattern
abstract class Repository<T, K = string> {
    protected httpService: HttpService<T>;
    
    constructor(httpService: HttpService<T>) {
        this.httpService = httpService;
    }
    
    abstract findById(id: K): Observable<T>;
    abstract save(entity: T): Observable<T>;
    abstract delete(id: K): Observable<void>;
}

// User repository
class UserRepository extends Repository<User, number> {
    constructor(httpService: HttpService<User>) {
        super(httpService);
    }
    
    findById(id: number): Observable<User> {
        return this.httpService.get(id.toString());
    }
    
    save(user: User): Observable<User> {
        return this.httpService.post(user);
    }
    
    delete(id: number): Observable<void> {
        return new Observable(observer => {
            fetch(`${this.httpService['config'].endpoint}/${id}`, {
                method: 'DELETE'
            })
            .then(() => {
                observer.next();
                observer.complete();
            })
            .catch(error => observer.error(error));
        });
    }
}

interface User {
    id: number;
    name: string;
    email: string;
    createdAt: Date;
}

console.log("Angular-like service test completed");
)";
    
    std::ofstream angularFile("angular_service_test.ts");
    if (angularFile.is_open()) {
        angularFile << angularTest;
        angularFile.close();
        std::cout << "Generated: angular_service_test.ts" << std::endl;
    }
    
    // Generate Vue-like composition test
    std::string vueTest = R"(
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
)";
    
    std::ofstream vueFile("vue_composition_test.ts");
    if (vueFile.is_open()) {
        vueFile << vueTest;
        vueFile.close();
        std::cout << "Generated: vue_composition_test.ts" << std::endl;
    }
    
    std::cout << "=== Complex TypeScript Test Generation Complete ===" << std::endl;
    return 0;
}