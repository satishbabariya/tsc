// Angular-like Service System Test
import {Inject, Injectable} from '@angular/core';
import {BehaviorSubject, Observable} from 'rxjs';

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

    constructor(@Inject('config') private config: ServiceConfig<T>) {
    }

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
                headers: {'Content-Type': 'application/json'},
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
