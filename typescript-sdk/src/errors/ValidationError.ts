import { DataApiError } from './DataApiError';

/**
 * 验证错误类
 */
export class ValidationError extends DataApiError {
  public readonly field?: string;
  public readonly value?: any;
  public readonly validationRules?: string[];
  
  constructor(
    message: string,
    field?: string,
    value?: any,
    validationRules?: string[],
    cause?: Error
  ) {
    super(message, 'VALIDATION_ERROR', 400, undefined, cause);
    this.name = 'ValidationError';
    this.field = field ?? '';
    this.value = value;
    this.validationRules = validationRules ?? [];
  }
  
  /**
   * 创建必填字段错误
   */
  static required(field: string): ValidationError {
    return new ValidationError(`Field '${field}' is required`, field, undefined, ['required']);
  }
  
  /**
   * 创建类型错误
   */
  static invalidType(field: string, expectedType: string, actualValue: any): ValidationError {
    return new ValidationError(
      `Field '${field}' must be of type '${expectedType}', got '${typeof actualValue}'`,
      field,
      actualValue,
      [`type:${expectedType}`]
    );
  }
  
  /**
   * 创建长度错误
   */
  static invalidLength(field: string, minLength?: number, maxLength?: number, actualLength?: number): ValidationError {
    let message = `Field '${field}' has invalid length`;
    const rules: string[] = [];
    
    if (minLength !== undefined) {
      message += ` (minimum: ${minLength})`;
      rules.push(`minLength:${minLength}`);
    }
    
    if (maxLength !== undefined) {
      message += ` (maximum: ${maxLength})`;
      rules.push(`maxLength:${maxLength}`);
    }
    
    if (actualLength !== undefined) {
      message += ` (actual: ${actualLength})`;
    }
    
    return new ValidationError(message, field, actualLength, rules);
  }
  
  /**
   * 创建格式错误
   */
  static invalidFormat(field: string, expectedFormat: string, actualValue: any): ValidationError {
    return new ValidationError(
      `Field '${field}' has invalid format, expected '${expectedFormat}'`,
      field,
      actualValue,
      [`format:${expectedFormat}`]
    );
  }
  
  /**
   * 创建范围错误
   */
  static outOfRange(field: string, min?: number, max?: number, actualValue?: any): ValidationError {
    let message = `Field '${field}' is out of range`;
    const rules: string[] = [];
    
    if (min !== undefined) {
      message += ` (minimum: ${min})`;
      rules.push(`min:${min}`);
    }
    
    if (max !== undefined) {
      message += ` (maximum: ${max})`;
      rules.push(`max:${max}`);
    }
    
    if (actualValue !== undefined) {
      message += ` (actual: ${actualValue})`;
    }
    
    return new ValidationError(message, field, actualValue, rules);
  }
  
  /**
   * 获取错误详情
   */
  getDetails(): Record<string, any> {
    return {
      ...super.getDetails(),
      field: this.field,
      value: this.value,
      validationRules: this.validationRules
    };
  }
  
  /**
   * 转换为JSON格式
   */
  override toJSON(): Record<string, any> {
    return {
      ...super.toJSON(),
      field: this.field,
      value: this.value,
      validationRules: this.validationRules
    };
  }
}